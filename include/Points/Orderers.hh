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

class IdxsComparator {
    std::vector<sf::Vector2f>* _v;
    public:
    IdxsComparator(): _v(nullptr) { }
    IdxsComparator(std::vector<sf::Vector2f>& v): _v(&v) { }

    bool operator()(size_t i, size_t j) const {
        auto& v = *_v;
        if(v[i].y < v[j].y)
            return true;
        else if(v[i].y > v[j].y)
            return false;
        else
            return v[i].x < v[j].x;
    }
};

class DeintersectorPointsOrderer : public PointsOrderer
{
  std::vector<sf::Vector2f>* _pts;
  std::vector<size_t> _idxs;
  /*!
   * indexes of start points of sections
   */
  std::vector<std::pair<size_t, size_t>> _intersects;
  std::multiset<size_t, IdxsComparator> _s;
  // IdxsComparator _cmp;

  void _init();

  void _clear() {
    _idxs.clear();
    _s.clear();
    _intersects.clear();
  }

  void _handleStartPoint(size_t idxA, size_t idxB);
  void _handleEndPoint(size_t idxA, size_t idxB);

  bool _checkIntersection(sf::Vector2f A, sf::Vector2f B,
                  sf::Vector2f C, sf::Vector2f D);
  bool _checkIntersection(size_t a, size_t b, size_t c, size_t d);
      
  void _checkStartingAt(size_t idx1st, size_t idx2nd);
  void _checkWithStartingAt(size_t idxA, size_t idxB, size_t idx2nd);

  void _find();
  void _remove();


public:

  DeintersectorPointsOrderer();

protected:

  virtual void compute() override;
};


class MSTPointsOrderer : public PointsOrderer
{
public:
  MSTPointsOrderer();

  DataInput<DelaunayTriangulation> graph;

protected:

  virtual void compute() override;
};


class SkipPointsOrderer : public PointsOrderer
{
public:
  SkipPointsOrderer();

  DataInput<DelaunayTriangulation> graph;

protected:

  virtual void compute() override;
};
