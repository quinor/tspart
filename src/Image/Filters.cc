#include "Image/Filters.hh"

ImageFilter::ImageFilter(Logger& log)
: logger(log)
, in(this)
, out(this)
{
}

void ImageFilter::compute()
{
  {
    std::ostringstream stream;
    stream<<"Applying filter: "<<filter_name;
    logger.enter(Logger::Level::Info, stream.str().c_str());    
  }

  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());
  sf::RenderTexture rtex;
  rtex.create(s.x, s.y);
  sf::RectangleShape rs(s);
  rs.setTextureRect(sf::IntRect(0, 0, 1, 1));
  frag.setUniform("tex", in.get_data());
  rtex.draw(rs, &frag);
  rtex.display();
  data_hook(out) = rtex.getTexture();

  logger.exit();
}


ImageFilterGrayscale::ImageFilterGrayscale(Logger& log)
: ImageFilter(log)
{
  filter_name = "Grayscale";
  if (!frag.loadFromFile("shaders/grayscale.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load shader");
    exit(-1);
  }
}

ImageFilterInverse::ImageFilterInverse(Logger& log)
: ImageFilter(log)
{
  filter_name = "Inverse";
  if (!frag.loadFromFile("shaders/inverse.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load shader");
    exit(-1);
  }
}


ImageFilterBlur::ImageFilterBlur(Logger& log)
: logger(log)
, blur_radius(0)
, in(this)
, out(this)
{
  if (!frag_x.loadFromFile("shaders/blur_x.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load X shader");
    exit(-1);
  }
  if (!frag_y.loadFromFile("shaders/blur_y.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load Y shader");
    exit(-1);
  }
}

void ImageFilterBlur::set_radius(size_t new_radius)
{
  refresh();
  blur_radius = new_radius;
}

void ImageFilterBlur::compute()
{
  logger.enter(Logger::Level::Info, "Applying filter: Blur");

  {
    std::ostringstream stream;
    stream<<"Blur radius: "<<blur_radius;
    logger.log(Logger::Level::Verbose, stream.str().c_str());    
  }

  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());

  sf::RenderTexture rt1, rt2;
  sf::Texture tex;
  rt1.create(s.x, s.y);
  rt2.create(s.x, s.y);
  frag_x.setUniform("radius", (int)blur_radius);
  frag_y.setUniform("radius", (int)blur_radius);
  sf::RectangleShape rs(s);
  rs.setTextureRect(sf::IntRect(0, 0, 1, 1));
  
  frag_x.setUniform("tex", in.get_data());
  rt1.draw(rs, &frag_x);
  rt1.display();

  tex = rt1.getTexture();

  frag_y.setUniform("tex", tex);
  rt2.draw(rs, &frag_y);
  rt2.display();

  tex = rt2.getTexture();

  frag_x.setUniform("tex", tex);
  rt1.draw(rs, &frag_x);
  rt1.display();

  tex = rt1.getTexture();

  frag_y.setUniform("tex", tex);
  rt2.draw(rs, &frag_y);
  rt2.display();

  data_hook(out) = rt2.getTexture();
  logger.exit();
}

