#include "FunctionAPI/API.hh"

API::API()
: logger(get_logger())
{}


API::~API()
{
  for (auto e : blocks)
    delete e;
}

void API::register_block(Block* bl)
{
  blocks.push_back(bl);
  bl->set_logger(logger);
}
