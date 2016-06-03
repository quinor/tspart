#include "Visualizer.hh"

void visualize_points_polygons(
  sf::Vector2u size,
  int scale_factor,
  const std::vector<sf::Vector2f>& points,
  const std::vector<std::vector<sf::Vector2f> >& polygons
)
{
  sf::RenderWindow window;
  window.create(
    sf::VideoMode(size.x/scale_factor/2+100, size.y/scale_factor/2+100, 32),
    "polyline visualization",
    sf::Style::Titlebar | sf::Style::Close
    );
  window.clear(sf::Color::White);

  {
    sf::VertexArray array(sf::Points, points.size());
    for (size_t i=0; i<points.size(); i++)
    {
      array[i].position=points[i]/float(scale_factor*2)+sf::Vector2f(50,50);
      array[i].color=sf::Color(192,0,0, 255);
    }
    window.draw(array);
  }

  for (auto polygon : polygons)
  {
    sf::VertexArray array(sf::Lines, polygon.size());
    for (size_t i=0; i<polygon.size(); i++)
    {
      array[i].position=polygon[i]/float(scale_factor*2)+sf::Vector2f(50,50);
    }
    for (size_t i=0; i<polygon.size(); i+=2)
    {
      array[i].color=sf::Color(0,0,255, 255);
      array[i+1].color=sf::Color(0,255,0, 255);
    }
    window.draw(array);
  }

  sf::Event evt;
  while (window.isOpen())
    if (window.pollEvent(evt) && evt.type == sf::Event::Closed)
      window.close();
    else
    {
      sf::sleep(sf::milliseconds(50));
      window.display();
    }
}

void visualize_polyline(
  sf::Vector2u size,
  int scale_factor,
  const std::vector<sf::Vector2f>& polyline
)
{
  sf::VertexArray array(sf::LinesStrip, polyline.size());
  for (size_t i=0; i<polyline.size(); i++)
  {
    array[i].position=polyline[i]/float(scale_factor*2)+sf::Vector2f(50,50);
    array[i].color=sf::Color(0,0,0, 255);
  }
  sf::RenderWindow window;
  window.create(
    sf::VideoMode(size.x/scale_factor/2+100, size.y/scale_factor/2+100, 32),
    "polyline visualization",
    sf::Style::Titlebar | sf::Style::Close
    );
  window.clear(sf::Color::White);
  window.draw(array);

  sf::Event evt;
  while (window.isOpen())
    if (window.pollEvent(evt) && evt.type == sf::Event::Closed)
      window.close();
    else
    {
      sf::sleep(sf::milliseconds(50));
      window.display();
    }
}
