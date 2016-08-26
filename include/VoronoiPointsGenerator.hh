#pragma once
#include "PointsGenerator.hh"

class VoronoiPointsGenerator : public PointsGenerator
{
public:
  using PointsGenerator::PointsGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, uint8_t*>);
private:
  struct MassElement;

  sf::Vector2u img_size;
  uint8_t* img_ptr;
  MassElement* mass_ptr;

  std::vector<sf::Vector2f> initial_distribution();
  std::vector<sf::Vector2f> redistribute(const std::vector<sf::Vector2f>&);
  sf::Vector2f make_centroid(sf::Vector2f,
    const std::vector<std::pair<sf::Vector2i, sf::Vector2i>>&);
  void calculate_mass_prefix_sums();
};
