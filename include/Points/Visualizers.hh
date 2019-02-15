#pragma once

#include <SFML/Graphics.hpp>

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"


class PolylineVisualizer : public Block, public AsOutput<sf::Texture, PolylineVisualizer>
{
public:
  PolylineVisualizer();

private:
  virtual void compute() override;

public:
  DataInput<Polyline> in;
  DataPromise<sf::Texture> out;
};


class VoronoiCellsVisualizer : public Block, public AsOutput<sf::Texture, VoronoiCellsVisualizer>
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


class DelaunayTriangulationVisualizer : public Block,
  public AsOutput<sf::Texture, DelaunayTriangulationVisualizer>
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


class PolygonVisualizer : public Block,
  public AsOutput<sf::Texture, PolygonVisualizer>
{
public:
  PolygonVisualizer();

private:
  virtual void compute () override;

public:
  DataInput<Polyline> polyline;
  DataInput<VoronoiCells> cells;
  DataInput<ColorMapping> colors;
  DataPromise<sf::Texture> out;
};
