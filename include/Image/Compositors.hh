#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

#include "Block.hh"
#include "Logger.hh"


class ImageCompositor : public Block
{
public:
  ImageCompositor(Logger& log);

protected:
  virtual void compute() override;
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
  void set_ratio(float ratio);
};
