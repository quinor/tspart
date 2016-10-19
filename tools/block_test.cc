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
  max.set_max_size(850);
  max.in.connect(load.out);
  
  ImageFilterGrayscale gray(get_logger());
  gray.in.connect(max.out);

  ImageFilterSigmoid sigm(get_logger());
  sigm.in.connect(gray.out);
  
  ImageFilterBlur bl(get_logger());
  bl.set_radius(30);
  bl.in.connect(sigm.out);

  ImageCompositorDifference diff(get_logger());
  diff.in1.connect(bl.out);
  diff.in2.connect(gray.out);

  ImageFilterSigmoid sigm2(get_logger());
  sigm2.set_shape(10, -128);
  sigm2.in.connect(diff.out);


  /*ImageViewer view(get_logger());
  view.in.connect(inv.out);
  view.update();*/

  ImageMultiViewer<3,2> view(get_logger());
  view.input(0,0).connect(max.out);
  view.caption(0, 0) = "Original image";
  view.input(1, 0).connect(gray.out);
  view.caption(1, 0) = "Grayscale'd";
  view.input(2, 0).connect(sigm.out);
  view.caption(2, 0) = "Sigmoid function";

  view.input(0, 1).connect(bl.out);
  view.caption(0, 1) = "Blurred";  
  view.input(1, 1).connect(diff.out);
  view.caption(1, 1) = "Blurred and regular difference";
  view.input(2, 1).connect(sigm2.out);
  view.caption(2, 1) = "Sigmoid function of the difference";
  view.update();

  return 0;
}
