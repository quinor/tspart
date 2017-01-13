#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

#include "utils/Block.hh"


class ImageFilter : public Block
{
public:
  ImageFilter();

protected:
  virtual void compute() override;
  virtual void prepare_shader();
  sf::Shader frag;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;
};

class ImageFilterGrayscale : public ImageFilter
{
public:
  ImageFilterGrayscale();
};

class ImageFilterInverse : public ImageFilter
{
public:
  ImageFilterInverse();
};


class ImageFilterSigmoid : public ImageFilter
{
public:
  ImageFilterSigmoid();

private:
  virtual void prepare_shader() override;

public:
  DataInput<std::pair<float, float>> shape_input;
  DataPromiseManual<std::pair<float, float>> shape_manual;
};


class ImageFilterGamma : public ImageFilter
{
public:
  ImageFilterGamma();

private:
  virtual void prepare_shader() override;

public:
  DataInput<float> shape_input;
  DataPromiseManual<float> shape_manual;
};


class ImageFilterLogarithm : public ImageFilter
{
public:
  ImageFilterLogarithm();

private:
  virtual void prepare_shader() override;

public:
  DataInput<float> shape_input;
  DataPromiseManual<float> shape_manual;
};


class ImageFilterGaussianBlur : public Block
{
public:
  ImageFilterGaussianBlur();

private:
  virtual void compute() override;
  sf::Shader frag_x;
  sf::Shader frag_y;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;

  DataInput<size_t> sigma_input;
  DataPromiseManual<size_t> sigma_manual;
};
