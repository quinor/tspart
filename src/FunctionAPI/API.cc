#include "FunctionAPI/API.hh"

API::API()
: logger(get_logger())
{}


API::~API()
{
  for (auto e : blocks)
    delete e;
}


template<typename T>
DataPromise<T>& API::input()
{
  auto ret = new Input<T>();
  blocks.push_back(ret);
  return ret->out;
}


template<typename T>
Output<T>& API::output(DataPromise<T>& src)
{
  auto ret = new Output<T>();
  ret->in.connect(src);
  blocks.push_back(ret);
  return *ret;
}
