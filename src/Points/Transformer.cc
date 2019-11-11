#include "Points/Transformer.hh"

#include <cmath>


TriangulationToCells::TriangulationToCells()
: triangles(this)
, centres(this)
, pts(this)
, cells(this)
{
  name = "TriangulationToCells";
}


void TriangulationToCells::compute()
{
  auto& in_triangles = triangles.get_data();
  auto& in_pts = centres.get_data();
  auto& out_pts = data_hook(pts);
  auto& out_cells = data_hook(cells);

  out_pts.size = in_pts.size;
  out_pts.scale = in_pts.scale;
  out_pts.pts.clear();

  out_cells.clear();

  for(auto& p : in_pts.pts)
  {
    auto neighbours = in_triangles.at(p);

    std::sort(
      neighbours.begin(),
      neighbours.end(),
      [=](const sf::Vector2f& a, const sf::Vector2f b) -> bool
      {
        auto da = a-p;
        auto db = b-p;
      return atan2(da.x, da.y) < atan2(db.x, db.y);
      }
    );
    for (unsigned i=0; i<neighbours.size(); i++)
    {
      auto x = p, y = neighbours[i], z = neighbours[(i+1)%neighbours.size()];
      auto m = (x+y+z)/3.f;
      if (x.x <= y.x && x.x <= z.x)
      {
        out_pts.pts.push_back(m);
        out_cells[m] = {};
        out_cells[m].emplace_back(std::make_pair(x, y));
        out_cells[m].emplace_back(std::make_pair(y, z));
        out_cells[m].emplace_back(std::make_pair(z, x));
      }
    }
  }
}
