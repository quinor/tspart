#pragma once

#include <glm/glm.hpp>
#include <cstdint>

typedef glm::tmat3x3<int, glm::precision::highp> imat3;
typedef glm::ivec3 ivec3;

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
