#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class ImageViewer : public Block
{
public:
  DataInput<sf::Texture> in;

  DataInput<std::string> window_name_input;
  Input<std::string> window_name_manual;

  ImageViewer();

protected:
  virtual void compute() override;
};
