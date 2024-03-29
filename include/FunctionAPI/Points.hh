#pragma once

#include "API.hh"
#include "Points/PointsTools.hh"

class PointsMixin : public virtual API
{
public:

  ColorImageToScalarField& color_image_to_scalar_field(DataPromise<sf::Texture>& data, Param<size_t> scale=2);
  GrayscaleImageToScalarField& grayscale_image_to_scalar_field(DataPromise<sf::Texture>& data, Param<size_t> scale=2);
  ScalarFieldMassPrefixSum& scalar_field_mass_prefix_sum(DataPromise<ScalarField<uint8_t>>& data);
  ScalarFieldColorPrefixSum& scalar_field_color_prefix_sum(DataPromise<ScalarField<sf::Color>>& data);

  PolylinePloterSaver& polyline_ploter_saver(DataPromise<Polyline>& data, Param<std::string> name=std::string(""));
  PolylineGcodeSaver& polyline_gcode_saver(DataPromise<Polyline>& data, Param<std::string> name=std::string(""));
  PolylineSVGSaver& polyline_svg_saver(DataPromise<Polyline>& data, Param<std::string> name=std::string(""));

  PointsGenerator& points_generator(DataPromise<ScalarField<uint8_t>>& data, Param<size_t> fill=12);
  PointsVoronoiDelaunay& points_voronoi_delaunay(DataPromise<Polyline>& data);
  TriangulationToCells& triangulation_to_cells(DataPromise<Polyline>& pts, DataPromise<Triangulation>& triangles);

  PointsRelaxator& points_relaxator(
    DataPromise<Polyline>& data,
    DataPromise<Cells>& cells,
    DataPromise<ScalarField<WeightedElement>>& mass_field);

  PointsColorAverager& points_color_averager(
    DataPromise<Polyline>& data,
    DataPromise<Cells>& cells,
    DataPromise<ScalarField<WeightedElement>>& color_field);

  HilbertPointsOrderer& hilbert_points_orderer(DataPromise<Polyline>& data);
  NearestNeighbourPointsOrderer& nearest_neighbour_points_orderer(DataPromise<Polyline>& data);
  DeintersectorPointsOrderer& deintersector_points_orderer(DataPromise<Polyline>& data);
  MSTPointsOrderer& mst_points_orderer(
    DataPromise<Polyline>& data,
    DataPromise<Triangulation>& graph,
    DataPromise<int>& shrink);
  SkipPointsOrderer& skip_points_orderer(
    DataPromise<Polyline>& data,
    DataPromise<Triangulation>& graph,
    DataPromise<int>& shrink);

  PolylineVisualizer& polyline_visualizer(DataPromise<Polyline>& data);
  CellsVisualizer& voronoi_cells_visualizer(
    DataPromise<Polyline>& data,
    DataPromise<Cells>& voronoi);
  TriangulationVisualizer& delaunay_triangulation_visualizer(
    DataPromise<Polyline>& data,
    DataPromise<Triangulation>& delaunay);

  PolygonVisualizer& polygon_visualizer(
    DataPromise<Polyline>& data,
    DataPromise<Cells>& voronoi,
    DataPromise<ColorMapping>& colors,
    Param<float> shrink=0.9f);

  DataPromise<Polyline>& n_voronoi_relaxation(
    DataPromise<Polyline>& data,
    DataPromise<ScalarField<WeightedElement>>& field,
    int n=1);

  DataPromise<Polyline>& mst_ordering(DataPromise<Polyline>& data, Param<int> shrink=0);
  DataPromise<Polyline>& skip_ordering(DataPromise<Polyline>& data, Param<int> shrink=0);
};
