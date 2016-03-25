#pragma once
#include <cstdint>

const double sigmoid_alpha = 30;
const double sigmoid_beta = -128;

const unsigned scale_factor = 4;
const int fill = 4;

const int threshold = scale_factor*scale_factor*256*fill;
