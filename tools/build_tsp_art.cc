#include "TSPBuilder.hh"
#include "Visualizer.hh"

int main (int argc, char** argv)
{
  get_logger().set_log_level(Logger::Level::Max);
  TSPBuilder
      <FileImageLoader,
      UpscaleImageProcessor,
      HilbertPointsGenerator,
      EmptyPolylineGenerator>
      builder;
  builder.config.load_from_args(argc, argv);

  std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    out = builder.build_image();
  visualize_polyline(out.first, out.second);
}