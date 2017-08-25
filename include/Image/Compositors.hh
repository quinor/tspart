#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <cstdlib>

#include "utils/Block.hh"


class ImageCompositor : public Block
{
public:
  ImageCompositor();

protected:
  virtual void compute() override;
  virtual void prepare_shader();
  sf::Shader frag;

public:
  DataInput<sf::Texture> in1;
  DataInput<sf::Texture> in2;
  DataPromise<sf::Texture> out;
};

class ImageCompositorAverage : public ImageCompositor
{
public:
  ImageCompositorAverage();

  DataInput<float> ratio_input;
  DataPromiseManual<float> ratio_manual;
private:
  virtual void prepare_shader() override;

};

class ImageCompositorDifference : public ImageCompositor
{
public:
  ImageCompositorDifference();
};

class ImageCompositorAbsoluteDifference : public ImageCompositor
{
public:
  ImageCompositorAbsoluteDifference();
};
