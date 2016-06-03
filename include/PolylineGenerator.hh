#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Config.hh"
#include "Logger.hh"


class PolylineGenerator
{
protected:
  Config& config;
  Logger& logger;
public:
  PolylineGenerator(Config& cfg, Logger& log)
  : config(cfg)
  , logger(log)
  {}
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>) = 0;
};

class EmptyPolylineGenerator : public PolylineGenerator
{
public:
  using PolylineGenerator::PolylineGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>);
};

class HilbertPolylineGenerator : public PolylineGenerator
{
public:
  using PolylineGenerator::PolylineGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>);
};
