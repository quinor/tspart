#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

#include "utils/Block.hh"
#include "utils/Logger.hh"


class ImageFilter : public Block
{
public:
  ImageFilter(Logger& log);

protected:
  virtual void compute() override;
  virtual void prepare_shader();
  std::string filter_name;
  sf::Shader frag;
  Logger& logger;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;
};

class ImageFilterGrayscale : public ImageFilter
{
public:
  ImageFilterGrayscale(Logger& log);
};

class ImageFilterInverse : public ImageFilter
{
public:
  ImageFilterInverse(Logger& log);
};


class ImageFilterSigmoid : public ImageFilter
{
public:
  ImageFilterSigmoid(Logger& log);

private:
  virtual void prepare_shader() override;

public:
  DataInput<std::pair<float, float>> shape_input;
  DataPromiseManual<std::pair<float, float>> shape_manual;
};


class ImageFilterGamma : public ImageFilter
{
public:
  ImageFilterGamma(Logger& log);

private:
  virtual void prepare_shader() override;

public:
  DataInput<float> shape_input;
  DataPromiseManual<float> shape_manual;
};


class ImageFilterLogarithm : public ImageFilter
{
public:
  ImageFilterLogarithm(Logger& log);

private:
  virtual void prepare_shader() override;

public:
  DataInput<float> shape_input;
  DataPromiseManual<float> shape_manual;
};


class ImageFilterBlur : public Block
{
public:
  ImageFilterBlur(Logger& log);

private:
  virtual void compute() override;
  sf::Shader frag_x;
  sf::Shader frag_y;
  Logger& logger;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;

  DataInput<size_t> radius_input;
  DataPromiseManual<size_t> radius_manual;
};
