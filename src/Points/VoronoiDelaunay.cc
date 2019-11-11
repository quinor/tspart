#include "Points/VoronoiDelaunay.hh"
#include "external/VoronoiDiagramGenerator.h"
#include <algorithm>
#include <cmath>


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

  sf::Vector2i grid_align(sf::Vector2f v)
  {
    return {(int)round(v.x), (int)round(v.y)};
  }

  size_t mul_hash(size_t a, size_t b)
  {
    return (a*(b+1))%1000000007;
  }
}


PointsVoronoiDelaunay::PointsVoronoiDelaunay()
: in(this)
, voronoi(this)
, delaunay(this)
{
  name = "PointsVoronoiDelaunay";
}

void PointsVoronoiDelaunay::compute()
{
  auto input = in.get_data();
  std::map<
    sf::Vector2f,
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>>,
    Vector2f_cmp
  > raw_cells;

  Cells cells;
  Triangulation graph;

  VoronoiDiagramGenerator generator;

  float *vx = new float[input.pts.size()];
  float *vy = new float[input.pts.size()];

  for (size_t i=0; i<input.pts.size(); i++)
  {
    vx[i] = input.pts[i].x;
    vy[i] = input.pts[i].y;
  }

  generator.generateVoronoi(vx, vy, input.pts.size(), 0, input.size.x-1, 0, input.size.y-1);
  delete[] vx;
  delete[] vy;

  sf::Vector2f e1, e2, p1, p2;
  generator.resetIterator();
  while (generator.getNext(
    e1.x, e1.y, e2.x, e2.y,
    p1.x, p1.y, p2.x, p2.y
  ))
  {

    if (raw_cells.find(p1) == raw_cells.end())
    {
      graph[p1]={};
      raw_cells[p1]={};
    }
    if (raw_cells.find(p2) == raw_cells.end())
    {
      graph[p2]={};
      raw_cells[p2]={};
    }

    raw_cells[p1].push_back(orientate(e1, e2, p1));
    raw_cells[p2].push_back(orientate(e1, e2, p2));
    graph[p1].push_back(p2);
    graph[p2].push_back(p1);
  }
  data_hook(delaunay) = std::move(graph);

  for (auto& e : raw_cells)
  {
    auto mid = e.first;
    auto& cell = cells[mid];
    cell = [&]() -> std::vector<Segment>
    {
      for (sf::Vector2f shake : {
        sf::Vector2f(0.f,0.f),
        sf::Vector2f(0.01234f, 0.06343f),
        sf::Vector2f(-0.042352f, 0.08976f)
      })
      {
        size_t pmul = 1, kmul = 1;
        std::vector<Segment> aligned;
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
          return aligned;
      }
      return std::vector<Segment>();
    }();

    auto angle_cmp = [&](const Segment& p1, const Segment& p2) -> bool
    {
      auto r1 = static_cast<sf::Vector2f>(p1.first)-mid;
      auto r2 = static_cast<sf::Vector2f>(p2.first)-mid;
      return atan2(r1.x, r1.y) < atan2(r2.x, r2.y);
    };

    std::sort(cell.begin(), cell.end(), angle_cmp);
  }

  data_hook(voronoi) = std::move(cells);
}


