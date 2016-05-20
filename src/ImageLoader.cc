#include <sstream>
#include "ImageLoader.hh"


sf::Image FileImageLoader::load()
{
  logger.enter(Logger::Level::Info, "Image loading");

  sf::Image tmp, img;
  if(!tmp.loadFromFile(config.filename))
  {
    logger.log(Logger::Level::Error, "Invalid image file!");
    exit(-1);
  }

  if (config.maximize)
  {
    logger.log(Logger::Level::Verbose, "Maximizing");
    sf::Vector2f s = sf::Vector2f(tmp.getSize());
    float scale = config.img_max_size/std::max(s.x, s.y);
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
    sf::Texture tex;
    tex.loadFromImage(tmp);
    sf::Sprite sp(tex);
    sp.scale({scale, scale});
    rtex.draw(sp);
    rtex.display();
    img = rtex.getTexture().copyToImage();
  }
  else
    img = tmp;

  logger.exit();
  return img;
}
