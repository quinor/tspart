#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"

#include <SFML/Graphics.hpp>
#include <string>


class ImageSaver : public Block
{
public:
  DataInput<sf::Texture> in;

  DataInput<std::string> filename_input;
  Input<std::string> filename_manual;

  ImageSaver();

protected:
  virtual void compute() override;
};
