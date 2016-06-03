#include "PointsGenerator.hh"
#include "HilbertGenerator.hh"
#include "VoronoiDiagramGenerator.h"

#include <map>

#include "Visualizer.hh"

std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
  HilbertPointsGenerator::generate(std::pair<sf::Vector2u, uint8_t*> in)
{
  logger.enter(Logger::Level::Info, "Creating basic points");

  int threshold = config.scale_factor*config.scale_factor*256*config.fill;

  auto img_size = in.first;
  auto img_ptr = in.second;

  int order = 0;
  while ((unsigned)(1<<order) < std::max(img_size.x, img_size.y))
    order++;

  HilbertGenerator generator(order);
  std::vector<sf::Vector2f> pts;

  auto progress = logger.progress(
    Logger::Level::Info,
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
  return {img_size, pts};
}


std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
  VoronoiPointsGenerator::generate(std::pair<sf::Vector2u, uint8_t*> in)
{
  logger.enter(Logger::Level::Info, "Generating points");
  auto img_size = in.first;
  auto img_ptr = in.second;

  auto pts = initial_distribution(img_size, img_ptr);

  auto progress = logger.progress(
    Logger::Level::Info,
    "Redistribution",
    config.number_of_passes
  );
  for (size_t i=0; i<config.number_of_passes; i++)
  {
    progress.update(i);
    pts = redistribute(img_size, pts);
  }

  logger.exit();
  return {img_size, pts};
}

std::vector<sf::Vector2f>
  VoronoiPointsGenerator::initial_distribution(sf::Vector2u img_size, uint8_t* img_ptr)
{
  int threshold = config.scale_factor*config.scale_factor*256*config.fill;
  logger.enter(Logger::Level::Info, "Creating initial distribution");

  int order = 0;
  while ((unsigned)(1<<order) < std::max(img_size.x, img_size.y))
    order++;

  HilbertGenerator generator(order);
  std::vector<sf::Vector2f> pts;

  auto progress = logger.progress(
    Logger::Level::Info,
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
  VoronoiPointsGenerator::redistribute(sf::Vector2u img_size, const std::vector<sf::Vector2f>& pts)
{
  logger.enter(Logger::Level::Verbose, "Redistributing points");

  VoronoiDiagramGenerator generator;

  float *vx = new float[pts.size()];
  float *vy = new float[pts.size()];

  for (size_t i=0; i<pts.size(); i++)
  {
    vx[i] = pts[i].x;
    vy[i] = pts[i].y;
  }

  logger.enter(Logger::Level::Verbose, "Voronoi diagram generation");
  generator.generateVoronoi(vx, vy, pts.size(), 0, img_size.x-1, 0, img_size.y-1);
  delete[] vx;
  delete[] vy;

  struct cmp
  {
    bool operator ()(const sf::Vector2f& a, const sf::Vector2f& b)
    {
      if (a.x != b.x)
        return a.x < b.x;
      return a.y < b.y;
    }
  };

  std::map<sf::Vector2f, std::vector<std::pair<sf::Vector2f, sf::Vector2f>>, cmp> cells;
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

  auto progress = logger.progress(
    Logger::Level::Verbose,
    "Redistribution pass",
    cells.size()
  );

  //std::vector<std::vector<sf::Vector2f>> perimeters;
  std::vector<sf::Vector2f> new_pts;
  int l = 0;
  for (auto& e : cells)
  {
    if (l%(cells.size()/10) == 0)
      progress.update(l);
    l++;

    sf::Vector2f sum(0,0);
    //perimeters.push_back({});
    for (auto p : e.second)
    {
      sum+=(p.second-p.first);
      //perimeters.back().push_back(p.first);
      //perimeters.back().push_back(p.second);
    }
    float l = sum.x*sum.x+sum.y*sum.y;
    if (l > 1)
      new_pts.push_back(e.first);
    else
      new_pts.push_back(make_centroid(e.first, e.second));
  }

  //visualize_points_polygons(img_size, config.scale_factor, pts, perimeters);

  logger.exit();
  return new_pts;
}

std::pair<sf::Vector2f, sf::Vector2f>
  VoronoiPointsGenerator::orientate(sf::Vector2f e1, sf::Vector2f e2, sf::Vector2f p)
{
  auto d1 = e2-e1;
  auto d2 = p-e1;
  e1 = p + (0.8f*(e1-p));
  e2 = p + (0.8f*(e2-p));
  if (d1.x*d2.y-d2.x*d1.y > 0)
    return {e1, e2};
  return {e2, e1};
}

sf::Vector2f VoronoiPointsGenerator::make_centroid(
  sf::Vector2f p,
  const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& perimeter
)
{

  MassElement m = {0,0,0};

  for (auto e : perimeter)
  {
    auto tmp = (e.first+e.second+p)/3.f;
    auto l1 = e.first-p;
    auto l2 = e.second-p;
    float w = (l1.x*l2.y-l1.y*l2.x);
    m+={
      tmp.x*w,
      tmp.y*w,
      w
    };
  }

  return m.middle();
}
