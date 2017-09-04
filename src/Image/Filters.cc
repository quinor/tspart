#include "Image/Filters.hh"
#include "utils/Config.hh"

ImageFilter::ImageFilter()
: in(this)
, out(this)
{
  name = "ImageFilter";
}

void ImageFilter::prepare_shader()
{}

void ImageFilter::compute()
{
  prepare_shader();

  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());
  sf::RenderTexture rtex;
  rtex.create(s.x, s.y);
  sf::RectangleShape rs(s);
  rs.setTextureRect(sf::IntRect(0, 0, 1, 1));
  frag.setUniform("tex", in.get_data());
  rtex.draw(rs, &frag);
  rtex.display();
  data_hook(out) = rtex.getTexture();
}


ImageFilterGrayscale::ImageFilterGrayscale()
{
  name = "ImageFilterGrayscale";
  if (!frag.loadFromFile(config.shader_pwd+"grayscale.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load grayscale shader";
    exit(-1);
  }
}

ImageFilterInverse::ImageFilterInverse()
{
  name = "ImageFilterInverse";
  if (!frag.loadFromFile(config.shader_pwd+"inverse.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load inverse shader";
    exit(-1);
  }
}


ImageFilterSigmoid::ImageFilterSigmoid()
: alpha_input(this)
, beta_input(this)
{
  name = "ImageFilterSigmoid";
  alpha_input.connect(alpha_manual);
  alpha_manual.set_data(30);
  beta_input.connect(beta_manual);
  beta_manual.set_data(128);
  if (!frag.loadFromFile(config.shader_pwd+"sigmoid.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load sigmoid shader";
    exit(-1);
  }
}

void ImageFilterSigmoid::prepare_shader()
{
  logger.log(Logger::Level::Verbose)<<"Sigmoid function: "<<"atan((x + "
    <<beta_input.get_data()<<") * "<<alpha_input.get_data()<<")";
  frag.setUniform("alpha", alpha_input.get_data());
  frag.setUniform("beta", beta_input.get_data());
}


ImageFilterGamma::ImageFilterGamma()
: shape_input(this)
{
  name = "ImageFilterGamma";
  shape_input.connect(shape_manual);
  shape_manual.set_data(1/2.2);
  if (!frag.loadFromFile(config.shader_pwd+"gamma.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load gamma shader";
    exit(-1);
  }
}

void ImageFilterGamma::prepare_shader()
{
  logger.log(Logger::Level::Verbose)<<"Gamma correction: x^"<<shape_input.get_data();
  frag.setUniform("power", shape_input.get_data());
}


ImageFilterLogarithm::ImageFilterLogarithm()
: shape_input(this)
{
  name = "ImageFilterLogarithm";
  shape_input.connect(shape_manual);
  shape_manual.set_data(10);
  if (!frag.loadFromFile(config.shader_pwd+"logarithm.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load logarithm shader";
    exit(-1);
  }
}

void ImageFilterLogarithm::prepare_shader()
{
  logger.log(Logger::Level::Verbose)<<"Log brightness correction: log(1+x/256*"
    <<shape_input.get_data()<<")/log(1+"<<shape_input.get_data()<<")";
  frag.setUniform("scale", shape_input.get_data());
}


ImageFilterGaussianBlur::ImageFilterGaussianBlur()
: in(this)
, out(this)
, sigma_input(this)
{
  name = "ImageFilterGaussianBlur";
  sigma_input.connect(sigma_manual);
  if (!frag_x.loadFromFile(config.shader_pwd+"blur_x.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load X shader";
    exit(-1);
  }
  if (!frag_y.loadFromFile(config.shader_pwd+"blur_y.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error)<<"Failed to load Y shader";
    exit(-1);
  }
}

void ImageFilterGaussianBlur::compute()
{
  logger.log(Logger::Level::Verbose)<<"Gaussian blur sigma: "<<sigma_input.get_data();

  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());

  sf::RenderTexture rt1, rt2;
  rt1.create(s.x, s.y);
  rt2.create(s.x, s.y);
  sf::RectangleShape rs(s);
  rs.setTextureRect(sf::IntRect(0, 0, 1, 1));

  frag_x.setUniform("sigma", (float)sigma_input.get_data());
  frag_x.setUniform("tex", in.get_data());
  rt1.draw(rs, &frag_x);
  rt1.display();

  sf::Texture tex = rt1.getTexture();

  frag_y.setUniform("sigma", (float)sigma_input.get_data());
  frag_y.setUniform("tex", tex);
  rt2.draw(rs, &frag_y);
  rt2.display();

  data_hook(out) = rt2.getTexture();
}

