#include "Points/PloterOutput.hh"
#include "Points/Generator.hh"
#include "utils/HilbertGenerator.hh"
#include <fstream>

PloterOutput::PloterOutput()
: in(this)
, filename_input(this)
{
  name = "PloterOutput";
  filename_input.connect(filename_manual);
}

void PloterOutput::compute()
{
  logger.log(Logger::Level::Verbose)<<"Filename: "<<filename_input.get_data();

  std::ofstream out;
  out.open(filename_input.get_data());

  auto& input = in.get_data();
  sf::Vector2f size = sf::Vector2f(input.size);

  logger.log(Logger::Level::Verbose)<<"Number of points saved: "<<input.pts.size();

  for (auto e : input.pts)
  {
    e-=size/2.f;
    e/=(float)input.scale;
    out<<(int)e.x<<" "<<(int)e.y<<"\n";
  }

  out.close();
}
