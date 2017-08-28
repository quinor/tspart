#include "Image/ImageTools.hh"
#include "Points/PointsTools.hh"
#include "utils/Logger.hh"
#include "utils/UtilityBlocks.hh"


int main (int argc, char** argv)
{
  if (argc != 2)
    return 0;

  get_logger().set_log_level(Logger::Level::Debug);

  Input<std::string> in;
  in.set_data(argv[1]);

  ImageLoader load;
  load.filename_input.connect(in);

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
  gam.shape_manual.set_data(10);


  ImageFilterInverse inv;
  inv.in.connect(gam);

  ImageToScalarField im_to_sc;
  im_to_sc.in.connect(inv);

  ScalarFieldMassPrefixSum mass;
  mass.in.connect(im_to_sc);

  PointsGenerator p_gen;
  p_gen.in.connect(im_to_sc);
  p_gen.fill_manual.set_data(12);

  PointsVoronoiDelaunay voronoi;
  voronoi.in.connect(p_gen);

  PointsRelaxator relax;
  relax.mass_field.connect(mass);
  relax.voronoi_cells.connect(voronoi.voronoi);
  relax.in.connect(p_gen);


  PointsVoronoiDelaunay voronoi2;
  voronoi2.in.connect(relax);

  MSTPointsOrderer gen;
  gen.in.connect(relax);
  gen.graph.connect(voronoi2.delaunay);

  PolylineVisualizer vis_poly;
  vis_poly.in.connect(gen);


  ImageFilterGaussianBlur bl3; //dirty trick for proper visualisation
  bl3.sigma_manual.set_data(1);
  bl3.in.connect(vis_poly);



  ImageMultiViewer<3,2> view;

  view.input(0, 0).connect(max);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(gray);
  view.caption_manual(0, 1).set_data("Grayscale'd");

  view.input(1, 0).connect(bl1);
  view.caption_manual(1, 0).set_data("Low blur");
  view.input(1, 1).connect(bl2);
  view.caption_manual(1, 1).set_data("High blur");

  view.input(2, 0).connect(sigm);
  view.caption_manual(2, 0).set_data("Sigmoid of blurs difference");
  view.input(2, 1).connect(bl3);
  view.caption_manual(2, 1).set_data("Polyline visualization");

  ImageSaver save;
  save.in.connect(vis_poly);
  save.filename_manual.set_data("out.jpg");

  PloterOutput pout;
  pout.in.connect(gen);
  pout.filename_manual.set_data("out.plt");

  pout.update();

  save.update();

//  view.update();
  mass.update();
  return 0;
}
