#include "Image/ImageTools.hh"
#include "Logger.hh"


int main (int argc, char** argv)
{
  if (argc == 1)
    return 0;

  get_logger().set_log_level(Logger::Level::Verbose);

  ImageLoader load(get_logger());
  load.set_filename(argv[1]);

  ImageMaximizer max(get_logger());
  max.set_max_size(1500);
  max.in.connect(load.out);
  
  ImageFilterGrayscale gray(get_logger());
  gray.in.connect(max.out);

  ImageFilterBlur bl1(get_logger());
  bl1.set_radius(5);
  bl1.in.connect(gray.out);

  ImageFilterBlur bl2(get_logger());
  bl2.set_radius(40);
  bl2.in.connect(gray.out);

  ImageCompositorDifference diff(get_logger());
  diff.in1.connect(bl1.out);
  diff.in2.connect(bl2.out);

  ImageFilterSigmoid sigm(get_logger());
  sigm.set_shape(10, -128);
  sigm.in.connect(diff.out);


  /*ImageViewer view(get_logger());
  view.in.connect(inv.out);
  view.update();*/

  ImageMultiViewer<3,2> view(get_logger());
  view.input(0, 0).connect(max.out);
  view.caption(0, 0) = "Original image";
  view.input(0, 1).connect(gray.out);
  view.caption(0, 1) = "Grayscale'd";  

  view.input(1, 0).connect(bl1.out);
  view.caption(1, 0) = "Low blur";
  view.input(1, 1).connect(bl2.out);
  view.caption(1, 1) = "High blur";

  view.input(2, 0).connect(diff.out);
  view.caption(2, 0) = "Low and high blur difference";  
  view.input(2, 1).connect(sigm.out);
  view.caption(2, 1) = "Sigmoid of difference";
  

  ImageSaver save(get_logger());
  save.in.connect(sigm.out);
  save.set_filename("out.jpg");

  save.update();

  view.update();
  return 0;
}
