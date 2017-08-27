#pragma once

#include "utils/Block.hh"
#include "DataTypes.hh"

class PointsRelaxator : public Block
{
public:

  PointsRelaxator();

private:

  virtual void compute() override;

public:

  DataInput<ScalarField<MassElement>> mass_field;
  DataInput<VoronoiCells> voronoi_cells;
  DataInput<Polyline> in;
  DataPromise<Polyline> out;
};
