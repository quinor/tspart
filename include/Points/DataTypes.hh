#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

namespace
{
  struct Vector2f_cmp
  {
    bool operator ()(const sf::Vector2f& a, const sf::Vector2f& b)
    {
      if (a.x != b.x)
        return a.x < b.x;
      return a.y < b.y;
    }
  };
}


class Polyline
{
public:
  Polyline()
  : size({0,0})
  , pts()
  , scale(0)
  {}
  sf::Vector2u size;
  std::vector<sf::Vector2f> pts;
  size_t scale;
};


class ScalarField
{
public:
  ScalarField()
  : size({0,0})
  , data(nullptr)
  , scale(0)
  {}

  ScalarField& operator=(ScalarField&& other)
  {
    std::swap(this->size, other.size);
    std::swap(this->data, other.data);
    std::swap(this->scale, other.scale);
    return *this;
  }

  explicit ScalarField(sf::Vector2u si, size_t sc)
  : size(unsigned(sc)*si)
  , scale(sc)
  {
    data = new uint8_t[size.x*size.y+7];
  }
  ~ScalarField()
  {
    delete[] data;
  }
  sf::Vector2u size;
  uint8_t* data;
  size_t scale;
};


using VoronoiCells =
  std::map<
    sf::Vector2f,
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>>,
    Vector2f_cmp
  >;


using DelaunayTriangulation =
  std::map<
    sf::Vector2f,
    std::vector<sf::Vector2f>,
    Vector2f_cmp
  >;
