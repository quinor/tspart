#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "utils/Block.hh"
#include "utils/Logger.hh"


class ImageLoader : public Block
{
public:
  DataPromise<sf::Texture> out;

  DataInput<std::string> filename_input;
  DataPromiseManual<std::string> filename_manual;

  ImageLoader();

protected:
  virtual void compute() override;
};
