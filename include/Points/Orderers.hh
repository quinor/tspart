#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"


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


class MSTPointsOrderer : public PointsOrderer
{
public:
  MSTPointsOrderer();

  DataInput<DelaunayTriangulation> graph;

protected:

  virtual void compute() override;
};
