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
  DataPromise<T>& input()
  {
    auto ret = new Input<T>();
    register_block(ret);
    return ret->out;
  }

  template<typename T>
  Output<T>& output(DataPromise<T>& src)
  {
    auto ret = new Output<T>();
    ret->in.connect(src);
    register_block(ret);
    return *ret;
  }

protected:
  void register_block(Block* bl);

public:

  Logger logger;

private:

  std::vector<Block*> blocks;
};
