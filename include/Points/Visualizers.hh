#pragma once

#include <SFML/Graphics.hpp>

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "DataTypes.hh"

class PolylineVisualizer : public Block
{
public:
  PolylineVisualizer(Logger& log);

private:
  virtual void compute() override;

private:
  Logger& logger;

public:
  DataInput<Polyline> in;
  DataPromise<sf::Texture> out;

};
