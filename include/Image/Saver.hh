#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "utils/Block.hh"


class ImageSaver : public Block
{
public:
  DataInput<sf::Texture> in;

  DataInput<std::string> filename_input;
  DataPromiseManual<std::string> filename_manual;

  ImageSaver();


protected:
  virtual void compute() override;
};
