#include <sstream>
#include "Image/Saver.hh"

ImageSaver::ImageSaver(Logger& log)
: in(this)
, logger(log)
{}

void ImageSaver::set_filename(std::string fname)
{
  refresh();
  filename = fname;
}

void ImageSaver::compute()
{
  logger.enter(Logger::Level::Info, "Image saving");
  {
    std::ostringstream stream;
    stream<<"Filename: "<<filename;
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }
  sf::Image tmp = in.get_data().copyToImage();
  if(!tmp.saveToFile(filename))
  {
    logger.log(Logger::Level::Error, "Couldn't save image!");
    exit(-1);
  }
  logger.exit();
}
