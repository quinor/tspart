#include "Image/ImageTools.hh"
#include "Points/PointsTools.hh"
#include "utils/Logger.hh"


int main (int argc, char** argv)
{
  if (argc == 1)
    return 0;

  get_logger().set_log_level(Logger::Level::Verbose);

  ImageLoader load(get_logger());
  load.filename_manual.set_data(argv[1]);

  ImageMaximizer max(get_logger());
  max.max_size_manual.set_data(1536);
  max.in.connect(load.out);
  
  ImageFilterGrayscale gray(get_logger());
  gray.in.connect(max.out);

  ImageFilterBlur bl1(get_logger());
  bl1.radius_manual.set_data(1);
  bl1.in.connect(gray.out);

  ImageFilterBlur bl2(get_logger());
  bl2.radius_manual.set_data(30);
  bl2.in.connect(gray.out);

  ImageCompositorDifference diff(get_logger());
  diff.in1.connect(bl1.out);
  diff.in2.connect(bl2.out);

  ImageFilterSigmoid sigm(get_logger());
  sigm.shape_manual.set_data({10,128});
  sigm.in.connect(diff.out);



  ImageFilterSigmoid sigm2(get_logger());
  sigm2.in.connect(sigm.out);
  sigm2.shape_manual.set_data({30,128});

  ImageFilterLogarithm gam(get_logger());
  gam.in.connect(sigm2.out);
  gam.shape_manual.set_data(10);


  ImageFilterInverse inv(get_logger());
  inv.in.connect(gam.out);

  ImageToScalarFieldConverter im_to_sc(get_logger());
  im_to_sc.in.connect(inv.out);

  PointsGenerator gen(get_logger());
  gen.in.connect(im_to_sc.out);
  gen.fill_manual.set_data(6);

  PolylineVisualizer vis_poly(get_logger());
  vis_poly.in.connect(gen.out);


  ImageFilterBlur bl3(get_logger()); //dirty trick for proper visualisation
  bl3.radius_manual.set_data(1);
  bl3.in.connect(vis_poly.out); 



  ImageMultiViewer<3,2> view(get_logger());
  view.window_size_manual.set_data({1200,675});

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
  
  ImageSaver save(get_logger());
  save.in.connect(vis_poly.out);
  save.filename_manual.set_data("out.jpg");

  save.update();

  view.update();
  return 0;
}
