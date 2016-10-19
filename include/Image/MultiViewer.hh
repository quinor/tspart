#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <boost/optional.hpp>

#include "Block.hh"
#include "Logger.hh"

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
  ImageMultiViewer(Logger& log);

  void set_window_name(std::string fname);
  void set_window_size(int w, int h);
  std::string& caption(int x, int y);
  DataInput<sf::Texture>& input(int x, int y);

protected:
  virtual void compute() override;

private:
  bool check_bonds(int x, int y);
  sf::Font font;

  Logger& logger;
  std::string window_name;
  sf::Vector2u window_size;
  boost::optional<DataInput<sf::Texture>> in[W][H];
  std::string capt[W][H];
  bool is_active[W][H];
};


template<int W, int H>
bool ImageMultiViewer<W, H>::check_bonds(int x, int y)
{
  return x >= 0 && y >= 0 && x < W && y < H;
}



template<int W, int H>
ImageMultiViewer<W, H>::ImageMultiViewer(Logger& log)
: logger(log)
, window_size(1280, 720)
, is_active()
{
  font.loadFromFile("misc/DejaVuSans.ttf");
}

template<int W, int H>
void ImageMultiViewer<W, H>::set_window_name(std::string name)
{
  window_name = name;
}

template<int W, int H>
void ImageMultiViewer<W, H>::set_window_size(int w, int h)
{
  window_size = {w, h};
}

template<int W, int H>
std::string& ImageMultiViewer<W, H>::caption(int x, int y)
{
  if (!check_bonds(x, y))
    throw "invalid x and y";
  return capt[x][y];
}

template<int W, int H>
DataInput<sf::Texture>& ImageMultiViewer<W, H>::input(int x, int y)
{
  if (!check_bonds(x, y))
    throw "invalid x and y";
  if (!in[x][y])
    in[x][y].emplace(this);
  return *(in[x][y]);
}

template<int W, int H>
void ImageMultiViewer<W, H>::compute()
{
  logger.enter(Logger::Level::Info, "Viewing images");

  sf::RenderWindow window;

  window.create(
    sf::VideoMode(window_size.x, window_size.y, 32),
    window_name,
    sf::Style::Titlebar | sf::Style::Close
    );
  window.clear(sf::Color::Black);

  auto pic_size = sf::Vector2u(window_size.x/W, window_size.y/H);
  for (int x=0; x<W; x++)
    for (int y=0; y<H; y++)
      if (in[x][y])
      {
        sf::Sprite sp(in[x][y]->get_data());
        auto ts = sp.getTexture()->getSize();
        sp.setOrigin(sf::Vector2f(ts)*0.5f);
        sp.setPosition(pic_size.x*(x+0.5), pic_size.y*(y+0.5)-15);
        float scale = std::min(float(pic_size.x)/ts.x, float(pic_size.y-CAPTION_SPACE_SIZE)/ts.y);
        sp.setScale(scale, scale);
        window.draw(sp);

        sf::Text text(capt[x][y], font, CAPTION_FONT_SIZE);
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

  logger.exit();
}
