#pragma once

#include <SFML/Graphics.hpp>
#include "Config.hh"
#include "Logger.hh"


class ImageLoader
{
protected:
  Config& config;
  Logger& logger;
public:
  ImageLoader(Config& cfg, Logger& log)
  : config(cfg)
  , logger(log)
  {}
  virtual sf::Image load() = 0;
};

class FileImageLoader : public ImageLoader
{
public:
  using ImageLoader::ImageLoader;
  virtual sf::Image load();
};
