#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Config.hh"
#include "Logger.hh"

class PointsGenerator
{
protected:
  Config& config;
  Logger& logger;
public:
  PointsGenerator(Config& cfg, Logger& log)
  : config(cfg)
  , logger(log)
  {}
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, uint8_t*>) = 0;
};

class HilbertPointsGenerator : public PointsGenerator
{
public:
  using PointsGenerator::PointsGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, uint8_t*>);
};
