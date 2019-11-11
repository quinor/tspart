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
  max.in.connect(load);

  ImageFilterGaussianBlur bl1;
  bl1.sigma_manual.set_data(1);
  bl1.in.connect(max);

  ImageFilterGaussianBlur bl2;
  bl2.sigma_manual.set_data(60);
  bl2.in.connect(max);

  ImageCompositorDifference diff;
  diff.in1.connect(bl1);
  diff.in2.connect(bl2);

  ImageFilterSigmoid sigm;
  sigm.alpha_manual.set_data(10);
  sigm.in.connect(diff);



  ImageFilterSigmoid sigm2;
  sigm2.in.connect(sigm);
  sigm2.alpha_manual.set_data(10);

  ImageFilterLogarithm gam;
  gam.in.connect(sigm2);
  gam.shape_manual.set_data(100);

  ImageFilterGrayscale gray;
  gray.in.connect(gam);

  ImageFilterInverse inv;
  inv.in.connect(gray);



  GrayscaleImageToScalarField im_to_sc;
  im_to_sc.in.connect(inv);

  PointsGenerator gen;
  gen.in.connect(im_to_sc);
  gen.fill_manual.set_data(200);

  PointsVoronoiDelaunay voronoi;
  voronoi.in.connect(gen);

  CellsVisualizer vis_voronoi;
  vis_voronoi.polyline.connect(gen);
  vis_voronoi.cells.connect(voronoi.voronoi);



  ImageSaver save;
  save.in.connect(vis_voronoi);
  save.filename_manual.set_data(argv[2]);

  save.update();

#ifdef DEBUG
  ImageFilterGaussianBlur bl3; //dirty trick for proper visualisation
  bl3.sigma_manual.set_data(1);
  bl3.in.connect(vis_voronoi);


  ImageMultiViewer<3,2> view;
  view.input(0, 0).connect(max);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(sigm);
  view.caption_manual(0, 1).set_data("Processed");

  view.input(1, 0).connect(sigm2);
  view.caption_manual(1, 0).set_data("2nd sigmoid");
  view.input(1, 1).connect(gam);
  view.caption_manual(1, 1).set_data("After gamma correction");

  view.input(2, 0).connect(gray);
  view.caption_manual(2, 0).set_data("Grayscale'd");
  view.input(2, 1).connect(bl3);
  view.caption_manual(2, 1).set_data("Final voronoi");

  view.update();
#endif

  return 0;
}
