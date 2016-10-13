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
  
  ImageFilterBlur bl(get_logger());
  bl.set_radius(50);
  bl.in.connect(gray.out);

  ImageCompositorAverage avg(get_logger());
  avg.set_ratio(-1);
  avg.in1.connect(gray.out);
  avg.in2.connect(bl.out);

  ImageFilterInverse inv(get_logger());
  inv.in.connect(avg.out);

  ImageViewer view1(get_logger());
  view1.in.connect(inv.out);
  view1.update();

  return 0;
}
