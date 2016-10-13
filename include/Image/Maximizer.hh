#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Block.hh"
#include "Logger.hh"


class ImageMaximizer : public Block
{
public:

  ImageMaximizer(Logger& log);

  void set_max_size(size_t size);

protected:
  virtual void compute() override;

private:
  Logger& logger;
  size_t max_size;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;
};
