#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Block.hh"
#include "Logger.hh"


class ImageSaver : public Block
{
public:
  DataInput<sf::Texture> in;

  ImageSaver(Logger& log);

  void set_filename(std::string fname);

protected:
  virtual void compute() override;

private:
  Logger& logger;
  std::string filename;

};
