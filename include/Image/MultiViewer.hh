#pragma once

#include "utils/UtilityBlocks.hh"
#include "utils/Block.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <optional>


namespace
{
  const int CAPTION_FONT_SIZE = 15;
  const int CAPTION_SPACE_SIZE = 30;
  const int CAPITON_PADDING = 5;
}

template<int W, int H>
class ImageMultiViewer : public Block
{
public:
  ImageMultiViewer();

  Input<std::string>& caption_manual(int x, int y);
  DataInput<std::string>& caption(int x, int y);
  DataInput<sf::Texture>& input(int x, int y);

  DataInput<std::string> window_name_input;
  Input<std::string> window_name_manual;

  DataInput<sf::Vector2u> window_size_input;
  Input<sf::Vector2u> window_size_manual;

protected:
  virtual void compute() override;

private:
  bool check_bonds(int x, int y);
  sf::Font font;

  std::optional<std::pair<
    DataInput<sf::Texture>,
    DataInput<std::string>
  >> in[W][H];
  Input<std::string> capt[W][H];
};


template<int W, int H>
bool ImageMultiViewer<W, H>::check_bonds(int x, int y)
{
  return x >= 0 && y >= 0 && x < W && y < H;
}

template<int W, int H>
ImageMultiViewer<W, H>::ImageMultiViewer()
: window_name_input(this)
, window_size_input(this)
{
  name = "ImageMultiViewer";
  window_size_manual.set_data({1600,900});
  window_name_input.connect(window_name_manual);
  window_size_input.connect(window_size_manual);
  font.loadFromFile("misc/DejaVuSans.ttf");
}

template<int W, int H>
Input<std::string>& ImageMultiViewer<W, H>::caption_manual(int x, int y)
{
  if (!check_bonds(x, y))
    throw "invalid x and y";
  return capt[x][y];
}

template<int W, int H>
DataInput<std::string>& ImageMultiViewer<W, H>::caption(int x, int y)
{
  if (!check_bonds(x, y))
    throw "invalid x and y";
  if (!in[x][y])
  {
    in[x][y].emplace(
      std::piecewise_construct,
      std::forward_as_tuple(this),
      std::forward_as_tuple(this)
    );
    in[x][y]->second.connect(capt[x][y]);
  }
  return in[x][y]->second;
}

template<int W, int H>
DataInput<sf::Texture>& ImageMultiViewer<W, H>::input(int x, int y)
{
  if (!check_bonds(x, y))
    throw "invalid x and y";
  if (!in[x][y])
  {
    in[x][y].emplace(
      std::piecewise_construct,
      std::forward_as_tuple(this),
      std::forward_as_tuple(this)
    );
    in[x][y]->second.connect(capt[x][y].out);
  }
  return in[x][y]->first;
}

template<int W, int H>
void ImageMultiViewer<W, H>::compute()
{
  sf::RenderWindow window;

  window.create(
    sf::VideoMode(window_size_input.get_data().x, window_size_input.get_data().y, 32),
    window_name_input.get_data(),
    sf::Style::Titlebar | sf::Style::Close
    );
  window.clear(sf::Color::Black);

  auto pic_size = sf::Vector2u(window_size_input.get_data().x/W, window_size_input.get_data().y/H);
  for (int x=0; x<W; x++)
    for (int y=0; y<H; y++)
      if (in[x][y])
      {
        sf::Sprite sp(in[x][y]->first.get_data());
        auto ts = sp.getTexture()->getSize();
        sp.setOrigin(sf::Vector2f(ts)*0.5f);
        sp.setPosition(pic_size.x*(x+0.5), pic_size.y*(y+0.5)-15);
        float scale = std::min(float(pic_size.x)/ts.x, float(pic_size.y-CAPTION_SPACE_SIZE)/ts.y);
        sp.setScale(scale, scale);
        window.draw(sp);

        sf::Text text(in[x][y]->second.get_data(), font, CAPTION_FONT_SIZE);
        text.setPosition(
          pic_size.x*x+2*CAPITON_PADDING,
          pic_size.y*(y+1)-CAPTION_SPACE_SIZE+CAPITON_PADDING
        );
        window.draw(text);

      }

  sf::Event evt;
  while (window.isOpen())
  {
    while (window.pollEvent(evt))
      if(evt.type == sf::Event::Closed)
        window.close();
    sf::sleep(sf::milliseconds(50));
    window.display();
  }

  refresh(); //so that window appears every time we update this block*/
}
