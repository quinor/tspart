#include "Image/ImageTools.hh"
#include "Points/PointsTools.hh"
#include "utils/Logger.hh"


int main (int argc, char** argv)
{
  if (argc != 3)
    return 0;

#ifdef DEBUG
  get_logger().set_log_level(Logger::Level::Verbose); //debug
#endif

  ImageLoader load;
  load.filename_manual.set_data(argv[1]);

  ImageMaximizer max;
  max.max_size_manual.set_data(1536);
  max.in.connect(load.out);

  ImageFilterGaussianBlur bl1;
  bl1.sigma_manual.set_data(1);
  bl1.in.connect(max.out);

  ImageFilterGaussianBlur bl2;
  bl2.sigma_manual.set_data(60);
  bl2.in.connect(max.out);

  ImageCompositorDifference diff;
  diff.in1.connect(bl1.out);
  diff.in2.connect(bl2.out);

  ImageFilterSigmoid sigm;
  sigm.shape_manual.set_data({10,128});
  sigm.in.connect(diff.out);



  ImageFilterSigmoid sigm2;
  sigm2.in.connect(sigm.out);
  sigm2.shape_manual.set_data({10,128});

  ImageFilterLogarithm gam;
  gam.in.connect(sigm2.out);
  gam.shape_manual.set_data(100);

  ImageFilterGrayscale gray;
  gray.in.connect(gam.out);

  ImageFilterInverse inv;
  inv.in.connect(gray.out);



  ImageToScalarFieldConverter im_to_sc;
  im_to_sc.in.connect(inv.out);

  PointsGenerator gen;
  gen.in.connect(im_to_sc.out);
  gen.fill_manual.set_data(200);

  PointsVoronoiDelaunay voronoi;
  voronoi.in.connect(gen.out);

  VoronoiCellsVisualizer vis_voronoi;
  vis_voronoi.polyline.connect(gen.out);
  vis_voronoi.cells.connect(voronoi.voronoi);



  ImageSaver save;
  save.in.connect(vis_voronoi.out);
  save.filename_manual.set_data(argv[2]);

  save.update();

#ifdef DEBUG
  ImageFilterGaussianBlur bl3; //dirty trick for proper visualisation
  bl3.sigma_manual.set_data(1);
  bl3.in.connect(vis_voronoi.out);


  ImageMultiViewer<3,2> view;
  view.input(0, 0).connect(max.out);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(sigm.out);
  view.caption_manual(0, 1).set_data("Processed");

  view.input(1, 0).connect(sigm2.out);
  view.caption_manual(1, 0).set_data("2nd sigmoid");
  view.input(1, 1).connect(gam.out);
  view.caption_manual(1, 1).set_data("After gamma correction");

  view.input(2, 0).connect(gray.out);
  view.caption_manual(2, 0).set_data("Grayscale'd");
  view.input(2, 1).connect(bl3.out);
  view.caption_manual(2, 1).set_data("Final voronoi");

  view.update();
#endif

  return 0;
}
