#include "PolylineGenerator.hh"

std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
EmptyPolylineGenerator::generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>> input)
{
  return input;
}
