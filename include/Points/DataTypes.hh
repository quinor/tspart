#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

struct Vector2f_cmp
{
  bool operator ()(const sf::Vector2f& a, const sf::Vector2f& b)
  {
    if (a.x != b.x)
      return a.x < b.x;
    return a.y < b.y;
  }
};


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


template <typename T>
class ScalarField
{
public:
  ScalarField()
  : size({0,0})
  , data(nullptr)
  , scale(0)
  {
    data++;
  }

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
    data = new T[size.x*size.y+7];
    data++;
  }
  ~ScalarField()
  {
    delete[] (data-1);
  }
  sf::Vector2u size;
  T* data;
  size_t scale;
};


class MassElement
{
public:

  MassElement (long long x, long long y, long long w = 1)
  : wx(x)
  , wy(y)
  , ws(w)
  {}

  MassElement ()
  : wx(0)
  , wy(0)
  , ws(0)
  {}

  MassElement operator + (const MassElement& other)
  {
    return {wx+other.wx, wy+other.wy, ws+other.ws};
  }

  MassElement operator - (const MassElement& other)
  {
    return {wx-other.wx, wy-other.wy, ws-other.ws};
  }

  template <typename Numeric>
  MassElement operator * (Numeric scalar)
  {
    static_assert(std::is_arithmetic<Numeric>::value, "You can multiply mass only by scalars!");
    return {wx*scalar, wy*scalar, ws*scalar};
  }

  template <typename Numeric>
  MassElement operator / (Numeric scalar)
  {
    static_assert(std::is_arithmetic<Numeric>::value, "You can multiply mass only by scalars!");
    return {wx/scalar, wy/scalar, ws/scalar};
  }

  MassElement operator += (const MassElement& other)
  {
    wx+=other.wx;
    wy+=other.wy;
    ws+=other.ws;
    return *this;
  }

  MassElement operator -= (const MassElement& other)
  {
    wx-=other.wx;
    wy-=other.wy;
    ws-=other.ws;
    return *this;
  }

  sf::Vector2f middle()
  {
    if (ws == 0)
      throw std::runtime_error("Empty Mass Element has been middle'd");
    return {float(wx)/ws, float(wy)/ws};
  }

  long long wx;
  long long wy;
  long long ws;

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
