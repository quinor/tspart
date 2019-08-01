#include "utils/HilbertGenerator.hh"

HilbertGenerator::HilbertGenerator (int depth)
: height(depth)
{
  levels = new uint8_t[height+1];
  matrices = new imat3[height+1];
  matrices[0] = {
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 }
  };
  levels[0]=0;

  for (size_t i = 1; i <= height; i++)
  {
    levels[i] = 0;
    matrices[i] = linalg::mul(
      imat3{
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 }
      },
      matrices[i - 1]
    );
  }
}

HilbertGenerator::~HilbertGenerator()
{
  delete [] levels;
  delete [] matrices;
}

ivec3 HilbertGenerator::get() const
{
  return linalg::mul(linalg::transpose(matrices[height]), ivec3(0, 0, 1));
}

ivec3 HilbertGenerator::next()
{
  size_t position = height;

  while (position > 0 && levels[position] == 3)
    position--;

  int r = 1 << (height - position);

  if (position != 0)
  {
    levels[position]++;

    switch (levels[position])
    {
    case 1:
      matrices[position] = linalg::mul(
        imat3{
          { 1, 0, 0 },
          { 0, 1, r },
          { 0, 0, 1 }
        },
        matrices[position - 1]
      );
      break;

    case 2:
      matrices[position] = linalg::mul(
        imat3{
          { 1, 0, r },
          { 0, 1, r },
          { 0, 0, 1 }
        },
        matrices[position - 1]
      );
      break;

    case 3:
      matrices[position] = linalg::mul(
        imat3{
          { 0, -1, 2 * r - 1 },
          { -1, 0, r - 1 },
          { 0, 0, 1 }
        },
        matrices[position - 1]
      );
      break;
    }
  }

  for (position++; position <= height; position++)
  {
    levels[position] = 0;
    matrices[position] = linalg::mul(
      imat3{
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 }
      },
      matrices[position - 1]
    );
  }

  return get();
}
