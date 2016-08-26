#pragma once

class Config
{
public:
  char filename[256];

  bool maximize;
  unsigned img_max_size;

  bool use_sigmoid;
  double sigmoid_alpha;
  double sigmoid_beta;

  unsigned scale_factor;

  unsigned fill;

  unsigned number_of_passes;

  unsigned log_level;


  Config();
  void load_from_args(int argc, char** argv);
};
