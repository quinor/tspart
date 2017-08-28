#include "FunctionAPI/API.hh"

API::API()
: logger(get_logger())
{}


API::~API()
{
  for (auto e : blocks)
    delete e;
}
