#include <iostream>
#include "Block.hh"

template <typename Content>
class DataProvider : public Block
{
public:
  DataPromise<Content> data;

  DataProvider()
  : data(this)
  {}

  Content& hook()
  {
    refresh();
    return data_hook(data);
  }

protected:

  virtual void compute() override
  {}
};

class NothingDoer : public Block
{
public:
  DataInput<int> in;
  DataPromise<int> out;

  NothingDoer()
  : in(this)
  , out(this)
  {}
protected:
  virtual void compute() override
  {
    data_hook(out) = in.get_data();
  }
};


int main ()
{
  NothingDoer nd;
  DataProvider<int> input;
  input.hook() = 1;
  input.refresh();

  nd.in.connect(input.data);
  nd.update();
  std::cout<<nd.out.get_data()<<"\n";

  input.hook() = 2;
  input.refresh();

  nd.update();
  std::cout<<nd.out.get_data()<<"\n";

  DataProvider<int> second_input;
  second_input.hook() = 3;
  second_input.refresh();

  nd.in.connect(second_input.data);
  nd.update();
  std::cout<<nd.out.get_data()<<"\n";
  return 0;
}
