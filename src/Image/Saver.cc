#include "Image/Saver.hh"

ImageSaver::ImageSaver()
: in(this)
, filename_input(this)
{
  name = "ImageSaver";
  filename_input.connect(filename_manual);
}

void ImageSaver::compute()
{
  logger.log(Logger::Level::Verbose)<<"Filename: "<<filename_input.get_data();
  sf::Image tmp = in.get_data().copyToImage();
  if(!tmp.saveToFile(filename_input.get_data()))
  {
    logger.log(Logger::Level::Error)<<"Couldn't save image!";
    exit(-1);
  }
}
