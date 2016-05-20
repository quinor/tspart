#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Logger.hh"

#include "ImageLoader.hh"
#include "ImageProcessor.hh"
#include "PointsGenerator.hh"
#include "PolylineGenerator.hh"

template <class Loader, class Processor, class Points, class Polyline>
class TSPBuilder
{
public:
  Config config;
private:
  Logger& logger;

  Loader loader;
  Processor processor;
  Points points;
  Polyline polyline;
public:
  TSPBuilder(Logger& log = get_logger())
  : logger   (log)
  , loader    (config, logger)
  , processor (config, logger)
  , points    (config, logger)
  , polyline  (config, logger)
  {}

  std::pair<sf::Vector2u, std::vector<sf::Vector2f>> build_image()
  {
    logger.enter(Logger::Level::Info, "TSPBuilder starting");
    auto img = loader.load();

    std::pair<sf::Vector2u, uint8_t*>
      img2 = processor.process(img);
    
    std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
      pts = points.generate(img2);

    delete [] img2.second;

    logger.exit();
    return polyline.generate(pts);
  }
};