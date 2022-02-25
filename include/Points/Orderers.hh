#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "Points/DataTypes.hh"
#include <set>


class PointsOrderer : public Block, public AsOutput<Polyline, PointsOrderer>
{
public:

  PointsOrderer();

  DataInput<Polyline> in;
  DataPromise<Polyline> out;
};


class HilbertPointsOrderer : public PointsOrderer
{
public:

  HilbertPointsOrderer();

protected:

  virtual void compute() override;
};


class NearestNeighbourPointsOrderer : public PointsOrderer
{
public:

  NearestNeighbourPointsOrderer();

protected:

  virtual void compute() override;
};

class IdxsComparator
{
  const std::vector<sf::Vector2f>* _v;
  const float* _x;

public:
  IdxsComparator(): _v(nullptr), _x(nullptr) { }
  IdxsComparator(const std::vector<sf::Vector2f>& v, float* x): _v(&v), _x(x) { }

  inline float get_y_at_x(const std::pair<size_t, size_t>& seg, float x0) const
  {
    auto& v = *_v;
    sf::Vector2f dist = v[seg.second] - v[seg.first];
    float y0 = (x0 - v[seg.first].x) * dist.y / dist.x + v[seg.first].y;
    return y0;
  }

  bool operator()(std::pair<size_t, size_t> i, std::pair<size_t, size_t> j) const
  {
    // the same section
    if(i.first == j.first && i.second == j.second)
      return false;
    float x = *_x;
    auto& v = *_v;
    if(x < v[i.first].x || x > v[i.second].x ||
       x < v[j.first].x || x > v[j.second].x)
      throw std::runtime_error("x coordinate outside section footprint");
    float eps = 1e-5;
    bool rev_ret = false;
    // both starting at x
    if(x == v[i.first].x && x == v[j.first].x)
      x = std::min(v[i.second].x, v[j.second].x);
    // both ending at x
    if(x == v[i.second].x && x == v[j.second].x) {
      x = std::max(v[i.first].x, v[j.first].x);
      rev_ret = true;
      eps *= -1;
    }
    float y_i = get_y_at_x(i, x);
    float y_j = get_y_at_x(j, x);
    if(y_i != y_j)
      return rev_ret != (y_i < y_j);
    y_i = get_y_at_x(i, x-eps);
    y_j = get_y_at_x(j, x-eps);
    if(y_i != y_j)
      return rev_ret != (y_i < y_j);
    throw std::runtime_error("segments overlapping");
  }
};

class DeintersectorPointsOrderer : public PointsOrderer
{
  struct Node {
    long _x;
    long _y;
    Node(size_t x, size_t y): _x(x), _y(y) { }

    bool contains(long elem)
    {
      return _x == elem || _y == elem;
    }

    bool replace(long elem, long value)
    {
      if (_x == elem) {
        _x = value;
        return true;
      }
      if (_y == elem) {
        _y = value;
        return true;
      }
      return false;
    }

    long next(long elem) const
    {
      if (_x == elem)
        return _y;
      if (_y == elem)
        return _x;
      throw std::invalid_argument("Node does not contain such element: "
                                  + std::to_string(elem));
    }
  };

  std::vector<Node> _swapper;
  const std::vector<sf::Vector2f>* _pts;
  std::vector<size_t> _idxs;
  /*!
   * indexes of start points of sections
   */
  std::vector<std::pair<size_t, size_t>> _intersects;
  float _current_x;
  std::set<std::pair<size_t, size_t>, IdxsComparator> _active;
  using ActiveIterT =
    std::set<std::pair<size_t, size_t>, IdxsComparator>::iterator;

  void _init_structs(const std::vector<sf::Vector2f>&);

  void _addIntersection(size_t idxA, size_t idxB)
  {
    _intersects.emplace_back(std::min(idxA, idxB),
                             std::max(idxA, idxB));
  }

  void _handleStartPoint(size_t idxA, size_t idxB);
  void _handleEndPoint(size_t idxA, size_t idxB);
  bool _handleIntersection(ActiveIterT seg1, ActiveIterT seg2, bool adv_rm=false);


  void _repin(long a1, long a2, long b1, long b2);


public:

  DeintersectorPointsOrderer();

  std::vector<std::pair<size_t, size_t>>& find(const std::vector<sf::Vector2f>&);
  void remove(std::vector<sf::Vector2f>*);

  bool static isIntersecting(sf::Vector2f A, sf::Vector2f B,
                             sf::Vector2f C, sf::Vector2f D);

  void clear()
  {
    _idxs.clear();
    _active.clear();
    _intersects.clear();
    _swapper.clear();
    _pts = nullptr;
  }

protected:

  virtual void compute() override;
};


class MSTPointsOrderer : public PointsOrderer
{
public:
  MSTPointsOrderer();

  DataInput<Triangulation> graph;
  DataInput<int> shrink;

protected:

  virtual void compute() override;
};


class SkipPointsOrderer : public PointsOrderer
{
public:
  SkipPointsOrderer();

  DataInput<Triangulation> graph;
  DataInput<int> shrink;

protected:

  virtual void compute() override;
};
