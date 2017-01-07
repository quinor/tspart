#include "Image/Compositors.hh"

ImageCompositor::ImageCompositor()
: in1(this)
, in2(this)
, out(this)
{
  name = "ImageCompositor";
}

void ImageCompositor::compute()
{
  if (in1.get_data().getSize() != in2.get_data().getSize())
  {
    logger.log(Logger::Level::Error, "Inputs differ in size!");
    exit(-1);
  }

  prepare_shader();

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
}

void ImageCompositor::prepare_shader()
{}


ImageCompositorAverage::ImageCompositorAverage()
: ImageCompositor()
, ratio_input(this)
{
  name = "ImageCompositorAverage";
  ratio_manual.set_data(0.5);
  ratio_input.connect(ratio_manual);
  if (!frag.loadFromFile("shaders/average.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load shader");
    exit(-1);
  }
}


ImageCompositorDifference::ImageCompositorDifference()
: ImageCompositor()
{
  name = "ImageCompositorDifference";
  if (!frag.loadFromFile("shaders/difference.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load shader");
    exit(-1);
  }
}


ImageCompositorAbsoluteDifference::ImageCompositorAbsoluteDifference()
: ImageCompositor()
{
  name = "ImageCompositorAbsoluteDifference";
  if (!frag.loadFromFile("shaders/abs_difference.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load shader");
    exit(-1);
  }
}

void ImageCompositorAverage::prepare_shader()
{
  frag.setUniform("ratio", ratio_input.get_data());
}