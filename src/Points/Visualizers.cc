#include "Points/Visualizers.hh"
#include <cmath>

PolylineVisualizer::PolylineVisualizer()
: in(this)
, out(this)
{
  name = "PolylineVisualizer";
}

void PolylineVisualizer::compute()
{
  auto& input = in.get_data();
  sf::Vector2f s = sf::Vector2f(input.size);
  sf::RenderTexture rtex;
  rtex.create(s.x/input.scale, s.y/input.scale);

  rtex.clear(sf::Color::White);

  sf::VertexArray array(sf::LinesStrip, input.pts.size());
  for (size_t i=0; i<input.pts.size(); i++)
  {
    array[i].position=input.pts[i]/float(input.scale);
    array[i].color=sf::Color(0,0,0, 255);
  }

  rtex.draw(array);
  rtex.display();
  data_hook(out) = rtex.getTexture();
}

namespace
{
  sf::Vector2f closen(sf::Vector2f center, sf::Vector2f point, float scale)
  {
    return (point-center)*scale+center;
  }
}

VoronoiCellsVisualizer::VoronoiCellsVisualizer()
: polyline(this)
, cells(this)
, out(this)
{
  name = "VoronoiCellsVisualizer";
}

void VoronoiCellsVisualizer::compute()
{
  auto& points = polyline.get_data();
  auto& cellmap = cells.get_data();

  sf::Vector2f s = sf::Vector2f(points.size);
  sf::RenderTexture rtex;
  rtex.create(s.x/points.scale, s.y/points.scale);

  rtex.clear(sf::Color::White);

  {
    sf::VertexArray array(sf::Points, points.pts.size());
    for (size_t i=0; i<points.pts.size(); i++)
    {
      array[i].position=points.pts[i]/float(points.scale);
      array[i].color=sf::Color::Red;
    }
    rtex.draw(array);
  }

  for (auto& e : cellmap)
  {
    auto& cell = e.second;
    sf::VertexArray array(sf::Lines, 2*cell.size());
    for (size_t i=0; i<cell.size(); i++)
    {
      array[2*i].position = closen(e.first, sf::Vector2f(cell[i].first), 0.9)/float(points.scale);
      array[2*i].color=sf::Color::Green;
      array[2*i+1].position = closen(e.first, sf::Vector2f(cell[i].second), 0.9)/float(points.scale);
      array[2*i+1].color=sf::Color::Blue;
    }
    rtex.draw(array);
  }

  rtex.display();
  data_hook(out) = rtex.getTexture();
}


DelaunayTriangulationVisualizer::DelaunayTriangulationVisualizer()
: polyline(this)
, neighbours(this)
, out(this)
{
  name = "DelaunayTriangulationVisualizer";
}

void DelaunayTriangulationVisualizer::compute()
{
  auto& points = polyline.get_data();
  auto& graph = neighbours.get_data();

  sf::Vector2f s = sf::Vector2f(points.size);
  sf::RenderTexture rtex;
  rtex.create(s.x/points.scale, s.y/points.scale);

  rtex.clear(sf::Color::White);


  for (auto& e : graph)
  {
    auto& vec = e.second;
    sf::VertexArray array(sf::Lines, 2*vec.size());
    for (size_t i=0; i<vec.size(); i++)
    {
      array[2*i].position = vec[i]/float(points.scale);
      array[2*i].color=sf::Color::Black;
      array[2*i+1].position = e.first/float(points.scale);
      array[2*i+1].color=sf::Color::Black;
    }
    rtex.draw(array);
  }

  rtex.display();
  data_hook(out) = rtex.getTexture();
}


PolygonVisualizer::PolygonVisualizer()
: polyline(this)
, cells(this)
, colors(this)
, out(this)
{
  name = "PolygonVisualizer";
}

void PolygonVisualizer::compute()
{
  auto& points = polyline.get_data();
  auto& cellmap = cells.get_data();
  auto& colormap = colors.get_data();

  sf::Vector2f s = sf::Vector2f(points.size);
  sf::RenderTexture rtex;
  rtex.create(s.x/points.scale, s.y/points.scale);

  rtex.clear(sf::Color::White);

  for (auto point : points.pts)
  {
    auto& cell = cellmap.at(point);
    auto color = colormap.at(point);
    sf::VertexArray array(sf::TriangleFan, cell.size());
    for (size_t i=0; i<cell.size(); i++)
    {
      auto pt = closen(point, sf::Vector2f(cell[i].first), 0.90)/float(points.scale);
      array[i].position = pt;
      array[i].color = color;
    }
    rtex.draw(array);
  }

  rtex.display();
  data_hook(out) = rtex.getTexture();
}

