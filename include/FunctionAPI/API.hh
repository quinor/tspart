#pragma once

#include "utils/Block.hh"
#include "utils/Logger.hh"
#include "utils/UtilityBlocks.hh"
#include <vector>
#include <type_traits>


class API
{
  template <typename T>
  friend class Param;

public:

  API();
  ~API();

protected:
  void register_block(Block* bl);

public:

  Logger logger;

private:

  std::vector<Block*> blocks;
};


template <typename T>
class Param
{
public:

  Param(T x)
  : in(new Input<T>(x))
  , pr(nullptr)
  , reg(true)
  {
  }

  template<typename X>
  Param(X& x)
  : in(nullptr)
  , pr(&(x.operator DataPromise<T>&()))
  , reg(false)
  {
  }

  Param (Param<T>&) = default;
  Param (Param<T>&&) = default;

  DataPromise<T>& get_input(API* a)
  {
    if (!reg)
      return *pr;
    a->register_block(in);
    return in->out;
  }

private:

  Input<T>* in;
  DataPromise<T>* pr;
  bool reg;
};
