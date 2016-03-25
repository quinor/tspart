#include "Visualizer.hh"
#include "Constants.hh"

void visualize_polyline(
  sf::Vector2u size,
  std::vector<sf::Vector2f> polyline
)
{
  sf::VertexArray curve(sf::LinesStrip, polyline.size());
  for (size_t i=0; i<polyline.size(); i++)
  {
    curve[i].position=polyline[i]/float(scale_factor*2)+sf::Vector2f(50,50);
    curve[i].color=sf::Color(80,80,80);
  }
  sf::RenderWindow window;
  window.create(
    sf::VideoMode(size.x/scale_factor/2+100, size.y/scale_factor/2+100, 32),
    "polyline visualization",
    sf::Style::Titlebar | sf::Style::Close
    );
  window.clear(sf::Color::White);
  window.draw(curve);
  window.display();
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