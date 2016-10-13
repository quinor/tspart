#include "Image/Compositors.hh"

ImageCompositor::ImageCompositor(Logger& log)
: logger(log)
, in1(this)
, in2(this)
, out(this)
{
}

void ImageCompositor::compute()
{
  {
    std::ostringstream stream;
    stream<<"Applying compositor: "<<compositor_name;
    logger.enter(Logger::Level::Info, stream.str().c_str());    
  }

  if (in1.get_data().getSize() != in2.get_data().getSize())
  {
    logger.log(Logger::Level::Error, "Inputs differ in size!");
    exit(-1);
  }

  sf::Vector2f s = sf::Vector2f(in1.get_data().getSize());
  sf::RenderTexture rtex;
  rtex.create(s.x, s.y);
  sf::RectangleShape rs(s);
  rs.setTextureRect(sf::IntRect(0, 0, 1, 1));
  frag.setUniform("tex1", in1.get_data());
  frag.setUniform("tex2", in2.get_data());
  rtex.draw(rs, &frag);
  rtex.display();
  data_hook(out) = rtex.getTexture();

  logger.exit();
}


ImageCompositorAverage::ImageCompositorAverage(Logger& log)
: ImageCompositor(log)
{
  compositor_name = "Average";
  if (!frag.loadFromFile("shaders/average.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load shader");
    exit(-1);
  }
  frag.setUniform("ratio", 0.5f);
}

void ImageCompositorAverage::set_ratio(float ratio)
{
  frag.setUniform("ratio", ratio);  
}