#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Block.hh"
#include "Logger.hh"

class ImageViewer : public Block
{
public:
  DataInput<sf::Texture> in;

  ImageViewer(Logger& log);

  void set_window_name(std::string fname);

protected:
  virtual void compute() override;

private:
  Logger& logger;
  std::string window_name;
};
