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
  if (!out.is_open())
  {
    logger.log(Logger::Level::Error)<<"Could not save the file! "<<filename_input.get_data();
    return;
  }

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


PolylineGcodeSaver::PolylineGcodeSaver()
: in(this)
, filename_input(this)
{
  name = "PolylineGcodeSaver";
  filename_input.connect(filename_manual);
}

void PolylineGcodeSaver::compute()
{
  logger.log(Logger::Level::Verbose)<<"Filename: "<<filename_input.get_data();

  std::ofstream out;
  out.open(filename_input.get_data());
  if (!out.is_open())
  {
    logger.log(Logger::Level::Error)<<"Could not save the file! "<<filename_input.get_data();
    return;
  }

  auto& input = in.get_data();
  logger.log(Logger::Level::Verbose)<<"Number of points saved: "<<input.pts.size();

  out<<"m107\n";
  out<<"g1 f700.00\n";
  out<<"g90\n";
  for (auto e : input.pts)
  {
    e/=(float)input.scale*10.f;
    out<<"g1 x"<<e.x<<" y"<<e.y<<"\n";
  }
  out<<"m106\n";

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
  if (!out.is_open())
  {
    logger.log(Logger::Level::Error)<<"Could not save the file! "<<filename_input.get_data();
    return;
  }

  auto& input = in.get_data();
  sf::Vector2f size = sf::Vector2f(input.size)/(float)input.scale;
  auto start = input.pts[0]/(float)(10.f*input.scale);

  logger.log(Logger::Level::Verbose)<<"Number of points saved: "<<input.pts.size();

  out
    << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
    << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
    << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    << "<svg height=\""<<size.y/10.f<<"mm\" width=\""<<size.x/10.f<<"mm\" "
    << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" "
    << "viewBox=\"0 0 "<<size.x/10.f<<" "<<size.y/10.f<<"\""<<">\n"
    << "<path d=\"M "<<start.x<<" "<<start.y<<"\n";

  for (size_t i=0; i<input.pts.size(); i++)
  {
    auto cur = input.pts[i]/(float)(10.f*input.scale);
    auto nxt = input.pts[std::min((size_t)i+1, input.pts.size()-1)]/(float)(10.f*input.scale);
    nxt = (nxt+cur)/2.f;
    out<<"Q "<<cur.x<<" "<<cur.y<<", "<<nxt.x<<" "<<nxt.y<<"\n";
    if (i%1000 == 0)
    {
      out<< "\" fill=\"none\" stroke=\"black\" stroke-width=\"0.4\"/>";
      out<< "<path d=\"M "<<nxt.x<<" "<<nxt.y<<"\n";
    }
  }

  out<< "\" fill=\"none\" stroke=\"black\" stroke-width=\"0.4\"/></svg>\n";

  out.close();
}
