#include <sstream>
#include "Image/Loader.hh"

ImageLoader::ImageLoader(Logger& log)
: out(this)
, logger(log)
{}

void ImageLoader::set_filename(std::string fname)
{
  refresh();
  filename = fname;
}

void ImageLoader::compute()
{
  logger.enter(Logger::Level::Info, "Image loading");
  {
    std::ostringstream stream;
    stream<<"Filename: "<<filename;
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }
  sf::Image tmp;
  if(!tmp.loadFromFile(filename))
  {
    logger.log(Logger::Level::Error, "Invalid image file!");
    exit(-1);
  }
  data_hook(out).loadFromImage(tmp);
  logger.exit();
}
