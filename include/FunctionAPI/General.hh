#pragma once

#include "API.hh"


class General : public virtual API
{
public:

  template<typename T>
  Input<T>& input(T val=T())
  {
    auto ret = new Input<T>();
    ret->set_data(val);
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
