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

class VoronoiPointsGenerator : public PointsGenerator
{
public:
  using PointsGenerator::PointsGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, uint8_t*>);
private:
  struct MassElement
  {
    float wx;
    float wy;
    float ws;

    MassElement operator + (const MassElement& other)
    {
      return {wx+other.wx, wy+other.wy, ws+other.ws};
    }

    MassElement operator += (const MassElement& other)
    {
      wx+=other.wx;
      wy+=other.wy;
      ws+=other.ws;
      return *this;
    }

    sf::Vector2f middle()
    {
      return {float(wx)/ws, float(wy)/ws};
    }
  };

  std::vector<sf::Vector2f> initial_distribution(sf::Vector2u, uint8_t*);
  std::vector<sf::Vector2f> redistribute(sf::Vector2u, const std::vector<sf::Vector2f>&);
  std::pair<sf::Vector2f, sf::Vector2f> orientate(sf::Vector2f, sf::Vector2f, sf::Vector2f);
  sf::Vector2f make_centroid(sf::Vector2f,
    const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>&);


};
