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


class VoronoiCellsVisualizer : public Block
{
public:
  VoronoiCellsVisualizer();

private:
  virtual void compute () override;

public:
  DataInput<Polyline> polyline;
  DataInput<VoronoiCells> cells;
  DataPromise<sf::Texture> out;
};


class DelaunayTriangulationVisualizer : public Block
{
public:
  DelaunayTriangulationVisualizer();

private:
  virtual void compute () override;

public:
  DataInput<Polyline> polyline;
  DataInput<DelaunayTriangulation> neighbours;
  DataPromise<sf::Texture> out;
};
