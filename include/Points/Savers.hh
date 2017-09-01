#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"


class PolylinePloterSaver : public Block
{
public:

  PolylinePloterSaver();

private:

  virtual void compute() override;

public:

  DataInput<Polyline> in;

  DataInput<std::string> filename_input;
  Input<std::string> filename_manual;
};


class PolylineSVGSaver : public Block
{
public:

  PolylineSVGSaver();

private:

  virtual void compute() override;

public:

  DataInput<Polyline> in;

  DataInput<std::string> filename_input;
  Input<std::string> filename_manual;
};
