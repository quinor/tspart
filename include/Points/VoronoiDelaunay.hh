#pragma once

#include <vector>
#include <map>

#include "utils/Block.hh"
#include "DataTypes.hh"


class PointsVoronoiDelaunay : public Block
{
public:

  PointsVoronoiDelaunay();

private:

  virtual void compute() override;

public:

  DataInput<Polyline> in;
  DataPromise<VoronoiCells> voronoi;
  DataPromise<DelaunayTriangulation> delaunay;
};
