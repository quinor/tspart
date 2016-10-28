#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "DataTypes.hh"

class PointsGenerator : public Block
{
public:
  PointsGenerator(Logger& log);

private:
  virtual void compute() override;

private:
  Logger& logger;

public:
  DataInput<ScalarField> in;
  DataPromise<Polyline> out;

  DataInput<size_t> fill_input;
  DataPromiseManual<size_t> fill_manual;
};
