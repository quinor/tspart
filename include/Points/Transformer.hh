#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"

class TriangulationToCells : public Block
{
public:

  TriangulationToCells();

private:

  virtual void compute() override;

public:

  DataInput<Triangulation> triangles;
  DataInput<Polyline> centres;
  DataPromise<Polyline> pts;
  DataPromise<Cells> cells;
};
