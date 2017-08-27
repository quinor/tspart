#pragma once

#include "utils/Block.hh"


template <typename T>
class Input : public Block
{
public:

  Input()
  : out(this)
  {
    name = "Input";
  }

  void set_data(T data)
  {
    data_hook(out) = data;
    refresh();
  }

  virtual void compute () override
  {}

public:

  DataPromise<T> out;
};


template <typename T>
class Output : public Block
{
public:

  Output()
  : in(this)
  {
    name = "Output";
  }

  const T& get_data()
  {
    update();
    return in.get_data();
  }

  virtual void compute () override
  {}

public:

  DataInput<T> in;
};
