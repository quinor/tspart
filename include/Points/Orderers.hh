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
  public:
  IdxsComparator(): _v(nullptr) { }
  IdxsComparator(const std::vector<sf::Vector2f>& v): _v(&v) { }

  bool operator()(std::pair<size_t, size_t> i, std::pair<size_t, size_t> j) const
  {
    auto& v = *_v;
    size_t ii = i.first, jj = j.first;

    // they start at different points
    if (ii != jj)
    {
      if (v[ii].y != v[jj].y)
        return v[ii].y < v[jj].y;
      return v[ii].x < v[jj].x;
    }

    // they start at the same point
    auto v1 = v[i.second]-v[i.first];
    auto v2 = v[j.second]-v[j.first];

    return v1.x*v2.y-v1.y*v2.x > 0;
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
  std::set<std::pair<size_t, size_t>, IdxsComparator> _active;

  void _init_structs(const std::vector<sf::Vector2f>&);

  void _addIntersection(size_t idxA, size_t idxB)
  {
    _intersects.emplace_back(std::min(idxA, idxB),
                             std::max(idxA, idxB));
  }

  void _handleStartPoint(size_t idxA, size_t idxB);
  void _handleEndPoint(size_t idxA, size_t idxB);
  void _checkIntersection(size_t idxA, size_t idxB, size_t idxC, size_t idxD);


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
