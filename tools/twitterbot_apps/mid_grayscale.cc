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

  ImageFilterGrayscale gray;
  gray.in.connect(sigm);

  ImageSaver save;
  save.in.connect(gray);
  save.filename_manual.set_data(argv[2]);

  save.update();

#ifdef DEBUG
  ImageMultiViewer<3,2> view;
  view.input(0, 0).connect(max);
  view.caption_manual(0, 0).set_data("Original image");
  view.input(0, 1).connect(bl1);
  view.caption_manual(0, 1).set_data("Low blur");

  view.input(1, 0).connect(bl2);
  view.caption_manual(1, 0).set_data("High blur");
  view.input(1, 1).connect(diff);
  view.caption_manual(1, 1).set_data("Low and high blur difference");

  view.input(2, 0).connect(sigm);
  view.caption_manual(2, 0).set_data("Sigmoid of difference");
  view.input(2, 1).connect(gray);
  view.caption_manual(2, 1).set_data("Grayscale'd");

  view.update();
#endif

  return 0;
}
