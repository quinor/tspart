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
  auto start = input.pts[0]/(float)input.scale;

  logger.log(Logger::Level::Verbose)<<"Number of points saved: "<<input.pts.size();

  out
    << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
    << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
    << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    << "<svg height=\""<<size.y<<"\" width=\""<<size.x<<"\" "
    << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
    << "<path d=\"M "<<start.x<<" "<<start.y<<" ";

  for (int i=0; i<input.pts.size(); i++)
  {
    auto cur = input.pts[i]/(float)input.scale;
    auto nxt = input.pts[std::min((size_t)i+1, input.pts.size()-1)]/(float)input.scale;
    nxt = (nxt+cur)/2.f;
    out<<"Q "<<cur.x<<" "<<cur.y<<", "<<nxt.x<<" "<<nxt.y<<" ";

  }

  out<< "\" fill=\"transparent\" stroke=\"black\" stroke-width=\"4\"/></svg>\n";

  out.close();
}
