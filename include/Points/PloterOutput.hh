#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"


class PloterOutput : public Block
{
public:

  PloterOutput();

private:

  virtual void compute() override;

public:

  DataInput<Polyline> in;

  DataInput<std::string> filename_input;
  Input<std::string> filename_manual;
};
