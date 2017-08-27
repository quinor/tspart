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
  load.filename_input.connect(in.out);

  ImageMaximizer max;
  max.max_size_manual.set_data(1536);
  max.in.connect(load.out);

  ImageFilterGrayscale gray;
  gray.in.connect(max.out);

  ImageFilterGaussianBlur bl1;
  bl1.sigma_manual.set_data(1);
  bl1.in.connect(gray.out);

  ImageFilterGaussianBlur bl2;
  bl2.sigma_manual.set_data(30);
  bl2.in.connect(gray.out);

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
  gam.shape_manual.set_data(10);


  ImageFilterInverse inv;
  inv.in.connect(gam.out);

  ImageToScalarFieldConverter im_to_sc;
  im_to_sc.in.connect(inv.out);

  ScalarFieldMassPrefixSum mass;
  mass.in.connect(im_to_sc.out);

  PointsGenerator p_gen;
  p_gen.in.connect(im_to_sc.out);
  p_gen.fill_manual.set_data(12);

  PointsVoronoiDelaunay voronoi;
  voronoi.in.connect(p_gen.out);

  PointsRelaxator relax;
  relax.mass_field.connect(mass.out);
  relax.voronoi_cells.connect(voronoi.voronoi);
  relax.in.connect(p_gen.out);


  PointsVoronoiDelaunay voronoi2;
  voronoi2.in.connect(relax.out);

  MSTPointsOrderer gen;
  gen.in.connect(relax.out);
  gen.graph.connect(voronoi2.delaunay);

  PolylineVisualizer vis_poly;
  vis_poly.in.connect(gen.out);


  ImageFilterGaussianBlur bl3; //dirty trick for proper visualisation
  bl3.sigma_manual.set_data(1);
  bl3.in.connect(vis_poly.out);



  ImageMultiViewer<3,2> view;

  view.input(0, 0).connect(max.out);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(gray.out);
  view.caption_manual(0, 1).set_data("Grayscale'd");

  view.input(1, 0).connect(bl1.out);
  view.caption_manual(1, 0).set_data("Low blur");
  view.input(1, 1).connect(bl2.out);
  view.caption_manual(1, 1).set_data("High blur");

  view.input(2, 0).connect(sigm.out);
  view.caption_manual(2, 0).set_data("Sigmoid of blurs difference");
  view.input(2, 1).connect(bl3.out);
  view.caption_manual(2, 1).set_data("Polyline visualization");

  ImageSaver save;
  save.in.connect(vis_poly.out);
  save.filename_manual.set_data("out.jpg");

  PloterOutput pout;
  pout.in.connect(gen.out);
  pout.filename_manual.set_data("out.plt");

  pout.update();

  save.update();

//  view.update();
  mass.update();
  return 0;
}
