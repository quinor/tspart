#pragma once

#include "external/linalg.h"
#include <cstdint>


using ivec3 = linalg::vec<int32_t, 3>;
using imat3 = linalg::mat<int32_t, 3, 3>;

class HilbertGenerator
{
private:
  size_t height;
  uint8_t* levels;
  imat3* matrices;

public:
  HilbertGenerator(int depth);
  ~HilbertGenerator();
  ivec3 get() const;
  ivec3 next();
};
