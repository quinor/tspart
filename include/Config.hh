#pragma once

class Config
{
public:
  char filename[256];

  bool maximize;
  float img_max_size;

  bool use_sigmoid;
  double sigmoid_alpha;
  double sigmoid_beta;

  unsigned scale_factor;

  unsigned fill;

  unsigned number_of_passes;


  Config();
  void load_from_args(int argc, char** argv);
};
