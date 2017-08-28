#pragma once

#include "API.hh"


class General : public virtual API
{
public:

  template<typename T>
  DataPromise<T>& input()
  {
    auto ret = new Input<T>();
    register_block(ret);
    return *ret;
  }

  template<typename T>
  Output<T>& output(DataPromise<T>& src)
  {
    auto ret = new Output<T>();
    ret->in.connect(src);
    register_block(ret);
    return *ret;
  }
};
