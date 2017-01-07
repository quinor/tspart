#pragma once

#include <SFML/Graphics.hpp>

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "DataTypes.hh"

class PolylineVisualizer : public Block
{
public:
  PolylineVisualizer();

private:
  virtual void compute() override;

public:
  DataInput<Polyline> in;
  DataPromise<sf::Texture> out;
};
