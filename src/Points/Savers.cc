#include "Points/Savers.hh"
#include <fstream>

PolylinePloterSaver::PolylinePloterSaver()
: in(this)
, filename_input(this)
{
  name = "PolylinePloterSaver";
  filename_input.connect(filename_manual);
}

void PolylinePloterSaver::compute()
{
  logger.log(Logger::Level::Verbose)<<"Filename: "<<filename_input.get_data();

  std::ofstream out;
  out.open(filename_input.get_data());

  auto& input = in.get_data();
  sf::Vector2f size = sf::Vector2f(input.size)/(float)input.scale;

  logger.log(Logger::Level::Verbose)<<"Number of points saved: "<<input.pts.size();

  for (auto e : input.pts)
  {
    e-=size/2.f;
    e/=(float)input.scale;
    out<<(int)e.x<<" "<<(int)e.y<<"\n";
  }

  out.close();
}


PolylineSVGSaver::PolylineSVGSaver()
: in(this)
, filename_input(this)
{
  name = "PolylineSVGSaver";
  filename_input.connect(filename_manual);
}

void PolylineSVGSaver::compute()
{
  logger.log(Logger::Level::Verbose)<<"Filename: "<<filename_input.get_data();

  std::ofstream out;
  out.open(filename_input.get_data());

  auto& input = in.get_data();
  sf::Vector2f size = sf::Vector2f(input.size)/(float)input.scale;

  logger.log(Logger::Level::Verbose)<<"Number of points saved: "<<input.pts.size();

  out
    << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
    << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
    << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    << "<svg height=\""<<size.y<<"\" width=\""<<size.x<<"\" "
    << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
    << "<polyline points=\"";

  for (auto e : input.pts)
  {
    e/=(float)input.scale;
    out<<(int)e.x<<","<<(int)e.y<<" ";
  }

  out
    << "\" style=\"fill:none;stroke:#000000;stroke-width:4\" /></svg>\n";

  out.close();
}
