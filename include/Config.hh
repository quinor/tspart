#pragma once

class Config
{
public:
  const char* filename;

  bool maximize;
  float img_max_size;

  bool use_sigmoid;
  double sigmoid_alpha;
  double sigmoid_beta;

  unsigned scale_factor;

  int fill;

  int number_of_passes;

  void load_from_args(int argc, char** argv);
};
