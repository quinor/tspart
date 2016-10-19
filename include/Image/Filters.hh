#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

#include "Block.hh"
#include "Logger.hh"


class ImageFilter : public Block
{
public:
  ImageFilter(Logger& log);

protected:
  virtual void compute() override;
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


class ImageFilterSigmoid : public Block
{
public:
  ImageFilterSigmoid(Logger& log);

  void set_shape(float a, float b);
private:
  virtual void compute() override;
  sf::Shader frag;
  Logger& logger;
  float alpha;
  float beta;
public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;
};


class ImageFilterBlur : public Block
{
public:
  ImageFilterBlur(Logger& log);

  void set_radius(size_t radius);

private:
  virtual void compute() override;
  sf::Shader frag_x;
  sf::Shader frag_y;
  Logger& logger;
  size_t blur_radius;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;
};
