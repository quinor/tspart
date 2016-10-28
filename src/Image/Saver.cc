#include <sstream>
#include "Image/Saver.hh"

ImageSaver::ImageSaver(Logger& log)
: in(this)
, filename_input(this)
, logger(log)
{
  filename_input.connect(filename_manual);
}

void ImageSaver::compute()
{
  logger.enter(Logger::Level::Info, "Image saving");
  {
    std::ostringstream stream;
    stream<<"Filename: "<<filename_input.get_data();
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }
  sf::Image tmp = in.get_data().copyToImage();
  if(!tmp.saveToFile(filename_input.get_data()))
  {
    logger.log(Logger::Level::Error, "Couldn't save image!");
    exit(-1);
  }
  logger.exit();
}
