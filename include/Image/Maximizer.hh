#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"

#include <SFML/Graphics.hpp>
#include <string>


class ImageMaximizer : public Block, public AsOutput<sf::Texture, ImageMaximizer>
{
public:

  ImageMaximizer();


private:
  virtual void compute() override;

public:
  DataInput<sf::Texture> in;
  DataPromise<sf::Texture> out;

  DataInput<size_t> max_size_input;
  Input<size_t> max_size_manual;
};
