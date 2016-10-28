#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "utils/Block.hh"
#include "utils/Logger.hh"


class ImageSaver : public Block
{
public:
  DataInput<sf::Texture> in;

  DataInput<std::string> filename_input;
  DataPromiseManual<std::string> filename_manual;

  ImageSaver(Logger& log);


protected:
  virtual void compute() override;

private:
  Logger& logger;

};
