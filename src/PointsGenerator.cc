#include "PointsGenerator.hh"
#include "HilbertGenerator.hh"


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
  return {img_size, pts};
}
