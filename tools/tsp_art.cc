#include "FunctionAPI/Graph.hh"



int main (int argc, char** argv)
{
  if (argc != 2)
    return 0;

  auto gr = Graph<ImageMixin, PointsMixin>();
  gr.logger.set_log_level(Logger::Level::Debug);

  auto vis = [&](DataPromise<Polyline>& poly) -> DataPromise<sf::Texture>&
    {return gr.image_filter_gaussian_blur(gr.polyline_visualizer(poly), 1);};
  PolylineVisualizer vis_poly;

  auto& in = gr.image_loader(argv[1]);
  auto& pre = gr.image_filter_logarithm(
    gr.image_normalization(
      gr.image_filter_grayscale(
        gr.image_maximizer(in, 2048)),
      2),
    10);

  auto& inv = gr.image_to_scalar_field(gr.image_filter_inverse(pre), 2);
  auto& pref = gr.scalar_field_mass_prefix_sum(inv);

  auto& pts = gr.points_generator(inv, 24);
  auto& relaxed = gr.n_voronoi_relaxation(pts, pref, 5);
  auto& path = gr.mst_ordering(relaxed);

  auto& path_vis = vis(path);
  auto& view = gr.image_multi_viewer<2, 2>();

  view.input(0, 0).connect(in);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(pre);
  view.caption_manual(0, 1).set_data("Preprocessing");

  view.input(1, 0).connect(vis(pts));
  view.caption_manual(1, 0).set_data("Generated points");
  view.input(1, 1).connect(path_vis);
  view.caption_manual(1, 1).set_data("After MST");

  auto& img_saver = gr.image_saver(path_vis, "out.jpg");
  auto& pts_saver = gr.points_saver(path, "out.plt");

  img_saver.update();
  pts_saver.update();
  view.update();
  pref.update();
  return 0;
}
