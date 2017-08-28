#pragma once

#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"

class PointsGenerator : public Block, public AsOutput<Polyline, PointsGenerator>
{
public:

  PointsGenerator();

private:

  virtual void compute() override;

public:

  DataInput<ScalarField<uint8_t>> in;
  DataPromise<Polyline> out;

  DataInput<size_t> fill_input;
  Input<size_t> fill_manual;
};
