#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include "Config.hh"
#include "Logger.hh"


class ImageProcessor
{
protected:
  Config& config;
  Logger& logger;
public:
  ImageProcessor(Config& cfg, Logger& log)
  : config(cfg)
  , logger(log)
  {}
  //caution! pointer returned by the function below has to be deleted manually!
  virtual std::pair<sf::Vector2u, uint8_t*> process(const sf::Image&) = 0;
};

class UpscaleImageProcessor : public ImageProcessor
{
  uint8_t* apply_filters(const sf::Image&);
  std::pair<sf::Vector2u, uint8_t*> upscale(sf::Vector2u, const uint8_t*);
public:
  using ImageProcessor::ImageProcessor;
  virtual std::pair<sf::Vector2u, uint8_t*> process(const sf::Image&);
};
