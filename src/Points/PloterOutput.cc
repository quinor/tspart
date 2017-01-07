#include "Points/PloterOutput.hh"

#include "Points/Generator.hh"
#include "utils/HilbertGenerator.hh"

#include <sstream>
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
  {
    std::ostringstream stream;
    stream<<"Filename: "<<filename_input.get_data();
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }

  std::ofstream out;
  out.open(filename_input.get_data());

  auto& input = in.get_data();
  sf::Vector2f size = sf::Vector2f(input.size);

  {
    std::ostringstream stream;
    stream<<"Number of points saved: "<<input.pts.size();
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }


  for (auto e : input.pts)
  {
    e-=size/2.f;
    e/=(float)input.scale;
    out<<(int)e.x<<" "<<(int)e.y<<"\n";
  }

  out.close();
}