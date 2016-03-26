/*
  Small Config module testing utility
*/

#include <cstdio>
#include "Config.hh"
#include "Logger.hh"
#include <argp.h>

int main (int argc, char** argv)
{
  Config c;
  c.load_from_args(argc, argv);

  printf("filename: %s\n", c.filename);
  printf("maximize: %d with size %f\n", c.maximize, c.img_max_size);
  printf("use_sigmoid: %d with values a=%f, b=%f\n",
    c.use_sigmoid, c.sigmoid_alpha, c.sigmoid_beta);
  printf("scale_factor: %d\n", c.scale_factor);
  printf("fill: %d\n", c.fill);
  printf("number_of_passes: %d\n", c.number_of_passes);

  return 0;
}
