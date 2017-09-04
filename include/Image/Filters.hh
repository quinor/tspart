#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"

#include <SFML/Graphics.hpp>
#include <string>
#include <cstdlib>


class ImageFilter : public Block, public AsOutput<sf::Texture, ImageFilter>
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
  DataInput<float> alpha_input;
  DataInput<float> beta_input;
  Input<float> alpha_manual;
  Input<float> beta_manual;
};


class ImageFilterGamma : public ImageFilter
{
public:
  ImageFilterGamma();

private:
  virtual void prepare_shader() override;

public:
  DataInput<float> shape_input;
  Input<float> shape_manual;
};


class ImageFilterLogarithm : public ImageFilter
{
public:
  ImageFilterLogarithm();

private:
  virtual void prepare_shader() override;

public:
  DataInput<float> shape_input;
  Input<float> shape_manual;
};


class ImageFilterGaussianBlur : public Block, public AsOutput<sf::Texture, ImageFilterGaussianBlur>
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
  Input<size_t> sigma_manual;
};
