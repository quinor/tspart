#include "Points/Averager.hh"


PointsColorAverager::PointsColorAverager()
: color_field(this)
, voronoi_cells(this)
, in(this)
, out(this)
{
  name = "PointsColorAverager";
}


void PointsColorAverager::compute()
{
  auto& input = in.get_data();
  auto& color = color_field.get_data();
  auto& cells = voronoi_cells.get_data();
  auto& output = data_hook(out);

  auto progress = logger.progress(Logger::Level::Verbose, "Color averaging", cells.size());

  int l = 0;
  for (auto& e : cells)
  {
    if (l%(cells.size()/10) == 0)
      progress.update(l);
    l++;

    WeightedElement m;

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
        m += color.data[y*input.size.x+x] * sgn;
        if (y*input.size.x+x > color.size.x*color.size.y)
          printf("wtf\n");
      }
    }

    output[e.first] = (m.ws == 0 ? sf::Color::White : m.color());
  }
}
