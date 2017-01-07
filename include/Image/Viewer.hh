#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "utils/Block.hh"
#include "utils/Logger.hh"

class ImageViewer : public Block
{
public:
  DataInput<sf::Texture> in;

  DataInput<std::string> window_name_input;
  DataPromiseManual<std::string> window_name_manual;

  ImageViewer();

protected:
  virtual void compute() override;
};
