#include "TSPBuilder.hh"
#include "Visualizer.hh"

int main (int argc, char** argv)
{
  get_logger().set_log_level(Logger::Level::Max);
  TSPBuilder
      <FileImageLoader,
      UpscaleImageProcessor,
      VoronoiPointsGenerator,
      HilbertPolylineGenerator>
      builder;
  builder.config.load_from_args(argc, argv);

  auto out = builder.build_image();
  visualize_polyline(out.first, builder.config.scale_factor, out.second);
}
