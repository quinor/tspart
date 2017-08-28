#pragma once

#include "API.hh"


class General : public API
{
public:

  template<typename T>
  DataPromise<T>& input()
  {
    auto ret = new Input<T>();
    blocks.push_back(ret);
    return *ret;
  }

  template<typename T>
  Output<T>& output(DataPromise<T>& src)
  {
    auto ret = new Output<T>();
    ret->in.connect(src);
    blocks.push_back(ret);
    return *ret;
  }
};
