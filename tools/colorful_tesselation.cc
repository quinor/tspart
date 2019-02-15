#include "FunctionAPI/Graph.hh"


int main (int argc, char** argv)
{
  if (argc != 3)
    exit(-1);

  auto gr = Graph<ImageMixin, PointsMixin>();
  gr.logger.set_log_level(Logger::Level::Verbose);

  auto& src = gr.input<std::string>();
  auto& dst = gr.input<std::string>();


  auto& in = gr.image_maximizer(gr.image_loader(src), 2048);
  auto& pre = gr.image_filter_sigmoid(
    gr.image_filter_logarithm(
      gr.image_filter_grayscale(
        gr.image_compositor_absolute_difference(
          gr.image_filter_gaussian_blur(in, 1),
          gr.image_filter_gaussian_blur(in, 25)
        )
      ),
      100.f
    ),
    2.5f
  );

  auto& inv = gr.grayscale_image_to_scalar_field(pre, 2);
  auto& pts = gr.n_voronoi_relaxation(
    gr.points_generator(inv, 100),
    gr.scalar_field_mass_prefix_sum(inv),
    5);

  auto& voronoi = gr.points_voronoi_delaunay(pts);
  auto& color_field = gr.scalar_field_color_prefix_sum(gr.color_image_to_scalar_field(in));
  auto& colors = gr.points_color_averager(pts, voronoi.voronoi, color_field);
  auto& color_voronoi = gr.polygon_visualizer(pts, voronoi.voronoi, colors);

  auto& saver = gr.image_saver(color_voronoi, dst);

  src.set_data(argv[1]);
  dst.set_data(argv[2]);
  saver.update();


  auto& view = gr.image_multi_viewer<2, 2>();

  view.input(0, 0).connect(in);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(pre);
  view.caption_manual(0, 1).set_data("Preprocessing");

  view.input(1, 0).connect(gr.image_filter_gaussian_blur(gr.voronoi_cells_visualizer(
    pts,
    voronoi.voronoi
  ), 1));
  view.caption_manual(1, 0).set_data("Voronoi tesselation");
  view.input(1, 1).connect(gr.image_filter_gaussian_blur(color_voronoi, 1));
  view.caption_manual(1, 1).set_data("Colored tesselation");

  auto& single_view = gr.image_viewer(color_voronoi);

  // for (int i=1; i<argc; i++)
  // {
  //   src.set_data(argv[i]);
  //   view.update();
  //   single_view.update();
  // }
  return 0;
}
