#include "Points/Generator.hh"
#include "utils/HilbertGenerator.hh"

PointsGenerator::PointsGenerator()
: in(this)
, out(this)
, fill_input(this)
{
  name = "PointsGenerator";
  fill_manual.set_data(6);
  fill_input.connect(fill_manual);
}

void PointsGenerator::compute()
{
  logger.log(Logger::Level::Verbose)<<"Fill is "<<fill_input.get_data();

  auto& input = in.get_data();

  auto& output = data_hook(out);
  output.size = input.size;
  output.scale = input.scale;
  output.pts = std::vector<sf::Vector2f>();

  int threshold = input.scale*input.scale*256*fill_input.get_data();

  int order = 0;
  while ((unsigned)(1<<order) < std::max(input.size.x, input.size.y))
    order++;

  HilbertGenerator generator(order);

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
    if (p.x>=(int)input.size.x || p.y>=(int)input.size.y)
      continue;
    val+=input.data[p.y*input.size.x+p.x];
    if (val > threshold)
    {
      val-=threshold;
      output.pts.emplace_back(p.x, p.y);
    }
  }
  progress.finish();
}
