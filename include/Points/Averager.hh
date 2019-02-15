#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"

class PointsColorAverager : public Block, public AsOutput<ColorMapping, PointsColorAverager>
{
public:

  PointsColorAverager();

private:

  virtual void compute() override;

public:

  DataInput<ScalarField<WeightedElement>> color_field;
  DataInput<VoronoiCells> voronoi_cells;
  DataInput<Polyline> in;
  DataPromise<ColorMapping> out;
};
