#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "utils/Block.hh"


class ImageMaximizer : public Block
{
public:

  ImageMaximizer();


private:
  virtual void compute() override;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;

  DataInput<size_t> max_size_input;
  DataPromiseManual<size_t> max_size_manual;
};
