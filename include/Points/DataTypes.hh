#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

struct Vector2f_cmp
{
  bool operator ()(const sf::Vector2f& a, const sf::Vector2f& b) const
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


class WeightedElement
{
public:

  WeightedElement (long long x, long long y, long long z = 0, long long w = 1)
  : wx(x)
  , wy(y)
  , wz(z)
  , ws(w)
  {}

  WeightedElement ()
  : wx(0)
  , wy(0)
  , wz(0)
  , ws(0)
  {}

  WeightedElement operator + (const WeightedElement& other)
  {
    return {wx+other.wx, wy+other.wy, wz+other.wz, ws+other.ws};
  }

  WeightedElement operator - (const WeightedElement& other)
  {
    return {wx-other.wx, wy-other.wz, wy-other.wz, ws-other.ws};
  }

  template <typename Numeric>
  WeightedElement operator * (Numeric scalar)
  {
    static_assert(std::is_arithmetic<Numeric>::value, "You can multiply mass only by scalars!");
    return {wx*scalar, wy*scalar, wz*scalar, ws*scalar};
  }

  template <typename Numeric>
  WeightedElement operator / (Numeric scalar)
  {
    static_assert(std::is_arithmetic<Numeric>::value, "You can multiply mass only by scalars!");
    return {wx/scalar, wy/scalar, wz/scalar, ws/scalar};
  }

  WeightedElement& operator += (const WeightedElement& other)
  {
    wx+=other.wx;
    wy+=other.wy;
    wz+=other.wz;
    ws+=other.ws;
    return *this;
  }

  WeightedElement& operator -= (const WeightedElement& other)
  {
    wx-=other.wx;
    wy-=other.wy;
    wz-=other.wz;
    ws-=other.ws;
    return *this;
  }

  sf::Vector2f middle()
  {
    if (ws == 0)
      throw std::runtime_error("Empty Mass Element has been middle'd");
    return {float(wx)/ws, float(wy)/ws};
  }

  sf::Color color()
  {
    if (ws == 0)
      throw std::runtime_error("Empty Mass Element has been middle'd");
    return {uint8_t(wx/ws), uint8_t(wy/ws), uint8_t(wz/ws)};

  }

  long long wx;
  long long wy;
  long long wz;
  long long ws;

};


using Segment = std::pair<sf::Vector2i, sf::Vector2i>;

using Cells =
  std::map<
    sf::Vector2f,
    std::vector<Segment>,
    Vector2f_cmp
  >;


using Triangulation =
  std::map<
    sf::Vector2f,
    std::vector<sf::Vector2f>,
    Vector2f_cmp
  >;


using ColorMapping =
  std::map<
    sf::Vector2f,
    sf::Color,
    Vector2f_cmp
  >;
