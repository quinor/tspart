#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"

#include <SFML/Graphics.hpp>
#include <string>


class ImageLoader : public Block, public AsOutput<sf::Texture, ImageLoader>
{
public:
  DataPromise<sf::Texture> out;

  DataInput<std::string> filename_input;
  Input<std::string> filename_manual;

  ImageLoader();

protected:
  virtual void compute() override;
};
