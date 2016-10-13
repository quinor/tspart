#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Block.hh"
#include "Logger.hh"


class ImageLoader : public Block
{
public:
  DataPromise<sf::Texture> out;

  ImageLoader(Logger& log);

  void set_filename(std::string fname);

protected:
  virtual void compute() override;

private:
  Logger& logger;
  std::string filename;

};
