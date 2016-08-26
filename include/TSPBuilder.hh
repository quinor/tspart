#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Config.hh"
#include "Logger.hh"

#include "ImageLoader.hh"
#include "ImageProcessor.hh"
#include "PointsGenerator.hh"
#include "VoronoiPointsGenerator.hh"
#include "PolylineGenerator.hh"

template <class Loader, class Processor, class Points, class Polyline>
class TSPBuilder
{
public:
  Config config;
private:
  Logger& logger;

  static_assert(std::is_base_of<ImageLoader, Loader>::value,
    "Loader doesn't inherit from base loader class");
  static_assert(std::is_base_of<ImageProcessor, Processor>::value,
    "Processor doesn't inherit from base processor class");
  static_assert(std::is_base_of<PointsGenerator, Points>::value,
    "Points generator doesn't inherit from base points generator class");
  static_assert(std::is_base_of<PolylineGenerator, Polyline>::value,
    "Polyline generator doesn't inherit from base polyline generator class");

public:
  TSPBuilder(Logger& log = get_logger())
  : logger (log)
  {}

  std::pair<sf::Vector2u, std::vector<sf::Vector2f>> build_image()
  {
    logger.set_log_level(std::vector<Logger::Level> {
      Logger::Level::None,
      Logger::Level::Error,
      Logger::Level::Warning,
      Logger::Level::Info,
      Logger::Level::Verbose,
      Logger::Level::Debug,
      Logger::Level::Max
    }[config.log_level]);

    logger.enter(Logger::Level::Info, "TSPBuilder");

    Loader    loader    (config, logger);
    Processor processor (config, logger);
    Points    points    (config, logger);
    Polyline  polyline  (config, logger);
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