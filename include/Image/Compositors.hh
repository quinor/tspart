#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

#include "utils/Block.hh"
#include "utils/Logger.hh"


class ImageCompositor : public Block
{
public:
  ImageCompositor(Logger& log);

protected:
  virtual void compute() override;
  virtual void prepare_shader();
  std::string compositor_name;
  sf::Shader frag;
  Logger& logger;

public:
  DataInput<sf::Texture> in1;
  DataInput<sf::Texture> in2;
  DataPromise<sf::Texture> out;
};

class ImageCompositorAverage : public ImageCompositor
{
public:
  ImageCompositorAverage(Logger& log);

  DataInput<float> ratio_input;
  DataPromiseManual<float> ratio_manual;
private:
  virtual void prepare_shader() override;

};

class ImageCompositorDifference : public ImageCompositor
{
public:
  ImageCompositorDifference(Logger& log);
};

class ImageCompositorAbsoluteDifference : public ImageCompositor
{
public:
  ImageCompositorAbsoluteDifference(Logger& log);
};
