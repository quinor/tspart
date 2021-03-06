#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"

class PointsRelaxator : public Block, public AsOutput<Polyline, PointsRelaxator>
{
public:

  PointsRelaxator();

private:

  virtual void compute() override;

public:

  DataInput<ScalarField<WeightedElement>> mass_field;
  DataInput<Cells> voronoi_cells;
  DataInput<Polyline> in;
  DataPromise<Polyline> out;
};
