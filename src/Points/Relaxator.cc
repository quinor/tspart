#include "Points/Relaxator.hh"


PointsRelaxator::PointsRelaxator()
: mass_field(this)
, voronoi_cells(this)
, in(this)
, out(this)
{
  name = "PointsRelaxator";
}


void PointsRelaxator::compute()
{
  auto& input = in.get_data();
  auto& mass = mass_field.get_data();
  auto& cells = voronoi_cells.get_data();
  auto& output = data_hook(out);
  output.size = input.size;
  output.scale = input.scale;

  auto progress = logger.progress(Logger::Level::Verbose, "Redistribution pass", cells.size());

  int l = 0;
  for (auto& e : cells)
  {
    if (l%(cells.size()/10) == 0)
      progress.update(l);
    l++;

    MassElement m;

    for (auto edge : e.second)
    {
      auto p = edge.first, k = edge.second;
      int sgn = 1;
      if (p.y > k.y)
      {
        std::swap(p, k);
        sgn*=-1;
      }
      auto shift = k-p;
      for (int i = 0; i < shift.y; i++)
      {
        int x = p.x + (shift.x*i)/shift.y;
        int y = p.y + i;
        m += mass.data[y*input.size.x+x] * sgn;
      }
    }

    output.pts.push_back(m.ws == 0 ? e.first : m.middle());
  }
}
