#include "Image/Filters.hh"

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
  if (!frag.loadFromFile("shaders/grayscale.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load grayscale shader");
    exit(-1);
  }
}

ImageFilterInverse::ImageFilterInverse()
{
  name = "ImageFilterInverse";
  if (!frag.loadFromFile("shaders/inverse.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load inverse shader");
    exit(-1);
  }
}


ImageFilterSigmoid::ImageFilterSigmoid()
: shape_input(this)
{
  name = "ImageFilterSigmoid";
  shape_input.connect(shape_manual);
  shape_manual.set_data({30, 128});
  if (!frag.loadFromFile("shaders/sigmoid.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load sigmoid shader");
    exit(-1);
  }
}

void ImageFilterSigmoid::prepare_shader()
{
  {
    std::ostringstream stream;
    stream<<"Sigmoid function: "
      <<"atan((x + "<<shape_input.get_data().second<<") * "<<shape_input.get_data().first<<")";
    logger.log(Logger::Level::Verbose, stream.str().c_str());    
  }
  frag.setUniform("alpha", shape_input.get_data().first);
  frag.setUniform("beta", shape_input.get_data().second);
}


ImageFilterGamma::ImageFilterGamma()
: shape_input(this)
{
  name = "ImageFilterGamma";
  shape_input.connect(shape_manual);
  shape_manual.set_data(1/2.2);
  if (!frag.loadFromFile("shaders/gamma.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load gamma shader");
    exit(-1);
  }
}

void ImageFilterGamma::prepare_shader()
{
  {
    std::ostringstream stream;
    stream<<"Gamma correction: x^"<<shape_input.get_data();
    logger.log(Logger::Level::Verbose, stream.str().c_str());    
  }

  frag.setUniform("power", shape_input.get_data());
}


ImageFilterLogarithm::ImageFilterLogarithm()
: shape_input(this)
{
  name = "ImageFilterLogarithm";
  shape_input.connect(shape_manual);
  shape_manual.set_data(10);
  if (!frag.loadFromFile("shaders/logarithm.frag", sf::Shader::Fragment))
  {
    logger.log(Logger::Level::Error, "Failed to load logarithm shader");
    exit(-1);
  }
}

void ImageFilterLogarithm::prepare_shader()
{
  {
    std::ostringstream stream;
    stream<<"Log brightness correction: log(1+x/256*"
      <<shape_input.get_data()<<")/log(1+"<<shape_input.get_data()<<")";
    logger.log(Logger::Level::Verbose, stream.str().c_str());    
  }

  frag.setUniform("scale", shape_input.get_data());
}


ImageFilterBlur::ImageFilterBlur()
: in(this)
, out(this)
, radius_input(this)
{
  name = "ImageFilterBlur";
  radius_input.connect(radius_manual);
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

void ImageFilterBlur::compute()
{

  {
    std::ostringstream stream;
    stream<<"Blur radius: "<<radius_input.get_data();
    logger.log(Logger::Level::Verbose, stream.str().c_str());    
  }

  sf::Vector2f s = sf::Vector2f(in.get_data().getSize());

  sf::RenderTexture rt1, rt2;
  sf::Texture tex;
  rt1.create(s.x, s.y);
  rt2.create(s.x, s.y);
  frag_x.setUniform("radius", (int)radius_input.get_data());
  frag_y.setUniform("radius", (int)radius_input.get_data());
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
}

