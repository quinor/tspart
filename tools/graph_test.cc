#include "FunctionAPI/Graph.hh"

int main (int argc, char** argv)
{
  get_logger().set_log_level(Logger::Level::Verbose);
  auto gr = Graph<ImageMixin>();

  auto& ld = gr.image_loader();

  auto& in = gr.image_filter_grayscale(gr.image_maximizer(ld, 1500));

  auto& diff = gr.image_filter_sigmoid(
    gr.image_compositor_difference(
      gr.image_filter_gaussian_blur(in, 1),
      gr.image_filter_gaussian_blur(in, 60)),
    {10,128});

  auto& view = gr.image_viewer(diff);

  for (int i=1; i<argc; i++)
  {
    ld.filename_manual.set_data(argv[i]);
    view.update();
  }
  return 0;
}
