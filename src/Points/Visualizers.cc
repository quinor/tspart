#include "Points/Visualizers.hh"

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