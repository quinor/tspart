#pragma once

#include "utils/Block.hh"
#include "DataTypes.hh"

class PointsGenerator : public Block
{
public:

  PointsGenerator();

private:

  virtual void compute() override;

public:

  DataInput<ScalarField> in;
  DataPromise<Polyline> out;

  DataInput<size_t> fill_input;
  DataPromiseManual<size_t> fill_manual;
};
