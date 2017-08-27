#pragma once

#include "utils/Block.hh"
#include "utils/Logger.hh"
#include "utils/UtilityBlocks.hh"
#include <vector>


class API
{
public:

  API();
  ~API();

  template<typename T>
  DataPromise<T>& input();

  template<typename T>
  Output<T>& output(DataPromise<T>& src);

public:

  Logger logger;

private:

  std::vector<Block*> blocks;
};
