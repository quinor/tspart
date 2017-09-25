#include "FunctionAPI/Graph.hh"


int main (int argc, char** argv)
{
  auto gr = Graph<ImageMixin>();

  auto& in = gr.input<std::string>();
  auto& img = gr.image_loader(in);
  auto& diff = gr.image_normalization(img, 30);
  auto& view = gr.image_viewer(gr.image_maximizer(diff, 800));

  for (int i=1; i<argc; i++)
  {
    in.set_data(std::string(argv[i]));
    view.update();
  }
  return 0;
}
