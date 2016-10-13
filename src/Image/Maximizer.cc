#include <sstream>
#include "Image/Maximizer.hh"

ImageMaximizer::ImageMaximizer(Logger& log)
: logger(log)
, in(this)
, out(this)
{}

void ImageMaximizer::set_max_size(size_t size)
{
  refresh();
  max_size = size;
}

void ImageMaximizer::compute()
{
  logger.enter(Logger::Level::Info, "Image maximizing");

  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());
  float scale = max_size/std::max(s.x, s.y);
  {
    std::ostringstream stream;
    stream<<"Size before maximizing: ("<<s.x<<", "<<s.y<<")";
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }
  s*=scale;
  {
    std::ostringstream stream;
    stream<<"Size after maximizing: ("<<s.x<<", "<<s.y<<")";
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }

  sf::RenderTexture rtex;
  rtex.create(s.x, s.y);
  sf::Sprite sp(in.get_data());
  sp.scale({scale, scale});
  rtex.draw(sp);
  rtex.display();
  data_hook(out) = rtex.getTexture();

  logger.exit();
}
