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


class CellsVisualizer : public Block, public AsOutput<sf::Texture, CellsVisualizer>
{
public:
  CellsVisualizer();

private:
  virtual void compute () override;

public:
  DataInput<Polyline> polyline;
  DataInput<Cells> cells;
  DataPromise<sf::Texture> out;
};


class TriangulationVisualizer : public Block,
  public AsOutput<sf::Texture, TriangulationVisualizer>
{
public:
  TriangulationVisualizer();

private:
  virtual void compute () override;

public:
  DataInput<Polyline> polyline;
  DataInput<Triangulation> neighbours;
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
  DataInput<Cells> cells;
  DataInput<ColorMapping> colors;
  DataPromise<sf::Texture> out;
};
