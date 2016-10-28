#include "Image/Viewer.hh"

ImageViewer::ImageViewer(Logger& log)
: in(this)
, window_name_input(this)
, logger(log)
{
  window_name_input.connect(window_name_manual);
}

void ImageViewer::compute()
{
  logger.enter(Logger::Level::Info, "Viewing image");
  sf::Vector2u size = in.get_data().getSize();
  sf::RenderWindow window;

  window.create(
    sf::VideoMode(size.x, size.y, 32),
    window_name_input.get_data(),
    sf::Style::Titlebar | sf::Style::Close
    );

  sf::Sprite sp(in.get_data());
  window.draw(sp);

  sf::Event evt;
  while (window.isOpen())
  {
    while (window.pollEvent(evt))
      if(evt.type == sf::Event::Closed)
        window.close();
    sf::sleep(sf::milliseconds(50));
    window.display();
  }
  
  refresh(); //so that window appears every time we update this block
  logger.exit();
}
