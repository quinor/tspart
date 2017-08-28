#include "Image/Maximizer.hh"

ImageMaximizer::ImageMaximizer()
: in(this)
, out(this)
, max_size_input(this)
{
  name = "ImageMaximizer";
  max_size_input.connect(max_size_manual);
  max_size_manual.set_data(1024);
}

void ImageMaximizer::compute()
{
  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());
  float scale = max_size_input.get_data()/std::max(s.x, s.y);

  logger.log(Logger::Level::Verbose)<<"Size before maximizing: ("<<s.x<<", "<<s.y<<")";
  s*=scale;
  logger.log(Logger::Level::Verbose)<<"Size after maximizing: ("<<s.x<<", "<<s.y<<")";

  sf::RenderTexture rtex;
  rtex.create(s.x, s.y);
  sf::Sprite sp(in.get_data());
  sp.scale({scale, scale});
  rtex.draw(sp);
  rtex.display();
  data_hook(out) = rtex.getTexture();
}
