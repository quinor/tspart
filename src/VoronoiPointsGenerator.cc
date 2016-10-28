#include "VoronoiPointsGenerator.hh"
#include "VoronoiDiagramGenerator.h"
#include "utils/HilbertGenerator.hh"

#include <map>
#include <algorithm>
#include <type_traits>

#include <cmath>
#include "Visualizer.hh"


namespace
{

  std::pair<sf::Vector2f, sf::Vector2f> orientate(sf::Vector2f e1, sf::Vector2f e2, sf::Vector2f p)
  {
    auto d1 = e2-e1;
    auto d2 = p-e1;
    if (d1.x*d2.y-d2.x*d1.y > 0)
      return {e1, e2};
    return {e2, e1};
  }

  struct Vector2f_cmp
  {
    bool operator ()(const sf::Vector2f& a, const sf::Vector2f& b)
    {
      if (a.x != b.x)
        return a.x < b.x;
      return a.y < b.y;
    }
  };

  sf::Vector2i grid_align(sf::Vector2f v)
  {
    return {(int)round(v.x), (int)round(v.y)};
  }

}


struct VoronoiPointsGenerator::MassElement
{
  long long wx;
  long long wy;
  long long ws;

  MassElement (long long x, long long y, long long w = 1)
  : wx(x)
  , wy(y)
  , ws(w)
  {}

  MassElement ()
  : wx(0)
  , wy(0)
  , ws(0)
  {}

  MassElement operator + (const MassElement& other)
  {
    return {wx+other.wx, wy+other.wy, ws+other.ws};
  }

  MassElement operator - (const MassElement& other)
  {
    return {wx-other.wx, wy-other.wy, ws-other.ws};
  }

  template <typename Numeric>
  MassElement operator * (Numeric scalar)
  {
    static_assert(std::is_arithmetic<Numeric>::value, "You can multiply mass only by scalars!");
    return {wx*scalar, wy*scalar, ws*scalar};
  }

  template <typename Numeric>
  MassElement operator / (Numeric scalar)
  {
    static_assert(std::is_arithmetic<Numeric>::value, "You can multiply mass only by scalars!");
    return {wx/scalar, wy/scalar, ws/scalar};
  }

  MassElement operator += (const MassElement& other)
  {
    wx+=other.wx;
    wy+=other.wy;
    ws+=other.ws;
    return *this;
  }

  MassElement operator -= (const MassElement& other)
  {
    wx-=other.wx;
    wy-=other.wy;
    ws-=other.ws;
    return *this;
  }

  sf::Vector2f middle()
  {
    if (ws == 0)
      throw std::runtime_error("Empty Mass Element has been middle'd");
    return {float(wx)/ws, float(wy)/ws};
  }
};


std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
  VoronoiPointsGenerator::generate(std::pair<sf::Vector2u, uint8_t*> in)
{
  logger.enter(Logger::Level::Info, "Generating points");
  img_size = in.first;
  img_ptr = in.second;

  calculate_mass_prefix_sums();

  auto pts = initial_distribution();

  auto progress = logger.progress(
    Logger::Level::Info,
    "Redistribution",
    config.number_of_passes
  );
  for (size_t i=0; i<config.number_of_passes; i++)
  {
    progress.update(i);
    pts = redistribute(pts);
  }

  delete[] (mass_ptr-1);

  logger.exit();
  return {img_size, pts};
}


std::vector<sf::Vector2f>
  VoronoiPointsGenerator::initial_distribution()
{
  int threshold = config.scale_factor*config.scale_factor*256*config.fill;
  logger.enter(Logger::Level::Info, "Creating initial distribution");

  int order = 0;
  while ((unsigned)(1<<order) < std::max(img_size.x, img_size.y))
    order++;

  HilbertGenerator generator(order);
  std::vector<sf::Vector2f> pts;

  auto progress = logger.progress(
    Logger::Level::Verbose,
    "Progress through Hilbert curve",
    1<<(2*order)
  );
  int val = 0;
  for (int i=0; i<(1<<(2*order)); i++)
  {
    if (i%(1<<(2*order-4)) == 0)
      progress.update(i);
    auto p = generator.get();
    generator.next();
    if (p.x>=(int)img_size.x || p.y>=(int)img_size.y)
      continue;
    val+=img_ptr[p.y*img_size.x+p.x];
    if (val > threshold)
    {
      val-=threshold;
      pts.emplace_back(p.x, p.y);
    }
  }
  progress.finish();

  logger.exit();
  return pts;
}


std::vector<sf::Vector2f>
  VoronoiPointsGenerator::redistribute(const std::vector<sf::Vector2f>& pts)
{
  logger.enter(Logger::Level::Verbose, "Redistributing points");


  //output of voronoi generation phase
  std::map<sf::Vector2f, std::vector<std::pair<sf::Vector2f, sf::Vector2f>>, Vector2f_cmp> cells;

  {
    logger.enter(Logger::Level::Verbose, "Voronoi diagram generation");

    VoronoiDiagramGenerator generator;

    float *vx = new float[pts.size()];
    float *vy = new float[pts.size()];

    for (size_t i=0; i<pts.size(); i++)
    {
      vx[i] = pts[i].x;
      vy[i] = pts[i].y;
    }

    generator.generateVoronoi(vx, vy, pts.size(), 0, img_size.x-1, 0, img_size.y-1);
    delete[] vx;
    delete[] vy;

    sf::Vector2f e1, e2, p1, p2;
    generator.resetIterator();
    while (generator.getNext(
      e1.x, e1.y, e2.x, e2.y,
      p1.x, p1.y, p2.x, p2.y
    ))
    {
      if (cells.find(p1) == cells.end())
        cells[p1]={};
      if (cells.find(p2) == cells.end())
        cells[p2]={};

      cells[p1].push_back(orientate(e1, e2, p1));
      cells[p2].push_back(orientate(e1, e2, p2));
    }
    logger.exit();
  }

  //output of redistribution phase
  std::vector<sf::Vector2f> new_pts;

  {
    auto progress = logger.progress(
      Logger::Level::Verbose,
      "Redistribution pass",
      cells.size()
    );

    int l = 0;
    for (auto& e : cells)
    {
      if (l%(cells.size()/10) == 0)
        progress.update(l);
      l++;

      new_pts.push_back([&]()
      {
        auto mul_hash = [](size_t a, size_t b){return (a*(b+1))%1000000007;};

        for (sf::Vector2f shake : {
          sf::Vector2f(0.f,0.f),
          sf::Vector2f(0.01234f, 0.06343f),
          sf::Vector2f(-0.042352f, 0.08976f)
        })
        {
          size_t pmul = 1, kmul = 1;
          std::vector<std::pair<sf::Vector2i, sf::Vector2i>> aligned;
          aligned.reserve(e.second.size());

          for (auto p : e.second)
          {
            auto
              u = grid_align(p.first+shake),
              v = grid_align(p.second+shake);

            pmul=mul_hash(pmul, v.x);
            pmul=mul_hash(pmul, v.y);
            kmul=mul_hash(kmul, u.x);
            kmul=mul_hash(kmul, u.y);
            aligned.push_back({u, v});
          }
          if (pmul == kmul)
            return make_centroid(e.first, aligned);
        }

        //failure
        return e.first;
      }());
    }
  }

  logger.exit();
  return new_pts;
}


void VoronoiPointsGenerator::calculate_mass_prefix_sums()
{
  mass_ptr = new MassElement[img_size.x*img_size.y+7];
  mass_ptr[0] = {0,0,0};
  mass_ptr++;

  for (size_t y=0; y<img_size.y; y++)
    for (size_t x=0; x<img_size.x; x++)
    {
      size_t pos = y*img_size.x+x;
      int weight = img_ptr[pos];
      mass_ptr[pos] = mass_ptr[pos-1]+MassElement(x, y)*weight;
    }
}


sf::Vector2f VoronoiPointsGenerator::make_centroid(
  sf::Vector2f p,
  const std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& perimeter
)
{
  MassElement m;

  for (auto e : perimeter)
  {
    auto p = e.first, k = e.second;
    int sgn = 1;
    if (p.y > k.y)
    {
      std::swap(p, k);
      sgn*=-1;
    }
//    printf ("edge (%s) from (%d, %d) to (%d, %d)\n", sgn == 1 ? "down" : " up ", e.first.x, e.first.y, e.second.x, e.second.y);
//    m = mass_ptr[p.y*img_size.x+p.x]-mass_ptr[p.y*img_size.x+p.x-1];
//    printf("mass element (%lld, %lld) %lld\n", m.wx, m.wy, m.ws);
    auto shift = k-p;
    for (int i = 0; i < shift.y; i++)
    {
      int x = p.x + (shift.x*i)/shift.y;
      int y = p.y + i;
      m += mass_ptr[y*img_size.x+x] * sgn;
    }
  }

//  exit(0);

  if (m.ws == 0)
    return p;
  else
    return m.middle();

/*  for (auto e : perimeter)
  {
    auto tmp = (sf::Vector2f(e.first+e.second)+p)/3.f;
    auto l1 = sf::Vector2f(e.first)-p;
    auto l2 = sf::Vector2f(e.second)-p;
    float w = (l1.x*l2.y-l1.y*l2.x);
    m+=MassElement(
      tmp.x,
      tmp.y
    )*w;
  }

  if (m.ws == 0)
    return p;
  else
    return m.middle();*/
}
