#include "FunctionAPI/Graph.hh"


int main (int argc, char** argv)
{
  auto gr = Graph<ImageMixin>();

  auto& in = gr.image_loader();
  auto& diff = gr.image_normalization(in, 30);
  auto& view = gr.image_viewer(gr.image_maximizer(diff, 800));

  for (int i=1; i<argc; i++)
  {
    in.filename_manual.set_data(argv[i]);
    view.update();
  }
  return 0;
}
