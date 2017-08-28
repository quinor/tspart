#include "Image/ImageTools.hh"
#include "Points/PointsTools.hh"
#include "utils/Logger.hh"


int main (int argc, char** argv)
{
  if (argc == 1)
    return 0;

  get_logger().set_log_level(Logger::Level::Verbose);

  ImageLoader load;
  load.filename_manual.set_data(argv[1]);

  ImageMaximizer max;
  max.max_size_manual.set_data(1536);
  max.in.connect(load);
  
  ImageFilterGrayscale gray;
  gray.in.connect(max);

  ImageFilterGaussianBlur bl1;
  bl1.sigma_manual.set_data(1);
  bl1.in.connect(gray);

  ImageFilterGaussianBlur bl2;
  bl2.sigma_manual.set_data(30);
  bl2.in.connect(gray);

  ImageCompositorDifference diff;
  diff.in1.connect(bl1);
  diff.in2.connect(bl2);

  ImageFilterSigmoid sigm;
  sigm.shape_manual.set_data({10,128});
  sigm.in.connect(diff);


  ImageFilterSigmoid sigm2;
  sigm2.in.connect(sigm);
  sigm2.shape_manual.set_data({10,128});

  ImageFilterLogarithm gam;
  gam.in.connect(sigm2);
  gam.shape_manual.set_data(0.1);


  ImageFilterInverse inv;
  inv.in.connect(gam);

  ImageToScalarFieldConverter im_to_sc;
  im_to_sc.in.connect(inv);

  PointsGenerator gen;
  gen.in.connect(im_to_sc);
  gen.fill_manual.set_data(500);

  PointsVoronoiDelaunay voronoi;
  voronoi.in.connect(gen);


  VoronoiCellsVisualizer vis_voronoi;
  vis_voronoi.polyline.connect(gen);
  vis_voronoi.cells.connect(voronoi.voronoi);

  ImageFilterGaussianBlur bl3; //dirty trick for proper visualisation
  bl3.sigma_manual.set_data(1);
  bl3.in.connect(vis_voronoi);


  DelaunayTriangulationVisualizer vis_delaunay;
  vis_delaunay.polyline.connect(gen);
  vis_delaunay.neighbours.connect(voronoi.delaunay);

  ImageFilterGaussianBlur bl4; //dirty trick for proper visualisation
  bl4.sigma_manual.set_data(1);
  bl4.in.connect(vis_delaunay);



  ImageMultiViewer<4,1> view;

  view.input(0, 0).connect(sigm);
  view.caption_manual(0, 0).set_data("Filtered");

  view.input(1, 0).connect(gam);
  view.caption_manual(1, 0).set_data("Altered for points generation");

  view.input(2, 0).connect(bl3);
  view.caption_manual(2, 0).set_data("Voronoi visualization");

  view.input(3, 0).connect(bl4);
  view.caption_manual(3, 0).set_data("Delaunay visualization");
  
  ImageSaver save1;
  save1.in.connect(vis_voronoi);
  save1.filename_manual.set_data("out1.jpg");

  ImageSaver save2;
  save2.in.connect(vis_delaunay);
  save2.filename_manual.set_data("out2.jpg");


  save1.update();
  save2.update();

  view.update();
  return 0;
}
