#include "Image/ImageTools.hh"
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

  ImageFilterGaussianBlur bl1;
  bl1.sigma_manual.set_data(1);
  bl1.in.connect(load.out);

  ImageFilterGaussianBlur bl2;
  bl2.sigma_manual.set_data(60);
  bl2.in.connect(load.out);

  ImageCompositorDifference diff;
  diff.in1.connect(bl1.out);
  diff.in2.connect(bl2.out);

  ImageFilterSigmoid sigm;
  sigm.shape_manual.set_data({10,128});
  sigm.in.connect(diff.out);

  ImageFilterGrayscale gray;
  gray.in.connect(sigm.out);

  ImageSaver save;
  save.in.connect(gray.out);
  save.filename_manual.set_data(argv[2]);

  save.update();

#ifdef DEBUG
  ImageMultiViewer<3,2> view;
  view.input(0, 0).connect(load.out);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(bl1.out);
  view.caption_manual(0, 1).set_data("Low blur");

  view.input(1, 0).connect(bl2.out);
  view.caption_manual(1, 0).set_data("High blur");
  view.input(1, 1).connect(diff.out);
  view.caption_manual(1, 1).set_data("Low and high blur difference");

  view.input(2, 0).connect(sigm.out);
  view.caption_manual(2, 0).set_data("Sigmoid of difference");
  view.input(2, 1).connect(gray.out);
  view.caption_manual(2, 1).set_data("Grayscale'd");

  view.update();
#endif

  return 0;
}
