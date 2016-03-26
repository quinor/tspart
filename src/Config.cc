#include "Config.hh"
#include "Logger.hh"
#include <argp.h>
#include <cstring>
#include <regex>

namespace {

argp_option options[] =
{
  {"maximize", 'm', "max_size", 0,
   "Specify size of longer edge of the image after it's rescaled. Defaults to 1500.", 2},
  {"no_maximize", 1001, 0, 0,
   "Don't maximize the image.", 3},
  {"transform", 't', "alpha,beta", 0,
   "Specify alpha and beta parameters of the sigmoid atan((x+beta)/alpha) "
   "transform which is applied to each pixel of the image after grayscale. "
   "Defaults to alpha=30, beta=-128.", 4},
  {"no_transform", 1002, 0, 0,
   "Don't use sigmoid transform.", 5},
  {"scale", 's', "scale_factor", 0,
   "Scale image this many times for the algorithm accuracy. Higher values "
   "lead to lower performance. Defaults to 4.", 6},
  {"fill", 'l', "fill", 0,
   "Density of the points. Value x means that fully black image will have "
   "1/x times pixel count points. Defaults to 4.", 7},
  {"passes", 'p', "passes", 0,
   "Number of passes relaxing part of the algorithm does. Defaults to 4.", 8},
  {}
};

const std::string re_float = "(-?[0-9]{1,5}(\\.[0-9]{1,3})?)";
const std::string re_int = "(-?[0-9]{1,5})";

//placeholder
error_t parse_opt (int key, char *arg, argp_state *state)
{
  Config* cfg = static_cast<Config*>(state->input);

  switch(key)
  {
    case 'm':
    {
      static const std::regex re("^"+re_float+"$", std::regex::optimize);
      std::smatch match;

      cfg->maximize = true;
      std::string tmp(arg);
      if (!std::regex_match(tmp, match, re))
        argp_error(state, "Invalid value for max_size");
      cfg->img_max_size = std::stof(match[1]);
      break;
    }
    case 1001:
    {
      cfg->maximize = false;
      break;
    }
    case 't':
    {
      static const std::regex re("^"+re_float+","+re_float+"$", std::regex::optimize);
      std::smatch match;

      cfg->use_sigmoid = true;
      std::string tmp(arg);
      if (!std::regex_match(tmp, match, re))
        argp_error(state, "Invalid values for sigmoid alpha and beta");
      cfg->sigmoid_alpha = std::stof(match[1]);
      cfg->sigmoid_beta = std::stof(match[3]);
      break;
    }
    case 1002:
    {
      cfg->use_sigmoid = false;
      break;
    }
    case 's':
    {
      static const std::regex re("^"+re_int+"$", std::regex::optimize);
      std::smatch match;

      std::string tmp(arg);
      if (!std::regex_match(tmp, match, re))
        argp_error(state, "Invalid scale factor");
      cfg->scale_factor = std::stoi(match[1]);
      break;
    }
    case 'l':
    {
      static const std::regex re("^"+re_int+"$", std::regex::optimize);
      std::smatch match;

      std::string tmp(arg);
      if (!std::regex_match(tmp, match, re))
        argp_error(state, "Invalid fill ratio");
      cfg->fill = std::stoi(match[1]);
      break;
    }
    case 'p':
    {
      static const std::regex re("^"+re_int+"$", std::regex::optimize);
      std::smatch match;

      std::string tmp(arg);
      if (!std::regex_match(tmp, match, re))
        argp_error(state, "Invalid number of passes");
      cfg->number_of_passes = std::stoi(match[1]);
      break;
    }
    case ARGP_KEY_ARG:
    {
      if (state->arg_num >= 1)
        argp_usage(state);
      strncpy(cfg->filename, arg, 255);
      cfg->filename[255]=0;
      break;
    }
    case ARGP_KEY_END:
    {
      if (state->arg_num < 1)
        argp_usage (state);
      break;
    }
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

const char* doc = 
  "A program converting images to paths that can be later drawn using ploter hardware.";

const char* arg_doc = 
  "FILENAME";

argp argp_struct
{
  options,
  parse_opt,
  arg_doc,
  doc,
  0,
  0,
  0
};

}

Config::Config()
: maximize(true)
, img_max_size(1500)
, use_sigmoid(true)
, sigmoid_alpha(30)
, sigmoid_beta(-128)
, scale_factor(4)
, fill(4)
, number_of_passes(4)
{}

void Config::load_from_args(int argc, char** argv)
{
  argp_parse(&argp_struct, argc, argv, 0, 0, this);
}
