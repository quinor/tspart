#pragma once

#include "API.hh"
#include "Points/PointsTools.hh"

class PointsMixin : public API
{
  ImageToScalarField& image_to_scalar_field(DataPromise<sf::Texture>& data, int scale);
  ScalarFieldMassPrefixSum& scalar_field_mass_prefix_sum(DataPromise<ScalarField<uint8_t>>& data);

  PointsGenerator& points_generator(DataPromise<ScalarField<uint8_t>>& data, int fill);
  PointsVoronoiDelaunay& points_voronoi_delaunay(DataPromise<Polyline>& data);

  PointsRelaxator& points_relaxator(
    DataPromise<Polyline>& data,
    DataPromise<VoronoiCells>& cells,
    DataPromise<ScalarField<MassElement>>& mass_field);

  HilbertPointsOrderer& hilbert_points_orderer(DataPromise<Polyline>& data);
  MSTPointsOrderer& mst_points_orderer(
    DataPromise<Polyline>& data,
    DataPromise<DelaunayTriangulation>& graph);

  PolylineVisualizer& polyline_visualizer(DataPromise<Polyline>& data);
  VoronoiCellsVisualizer& voronoi_cells_visualizer(
    DataPromise<Polyline>& data,
    DataPromise<VoronoiCells>& voronoi);
  DelaunayTriangulationVisualizer& delaunay_triangulation_visualizer(
    DataPromise<Polyline>& data,
    DataPromise<DelaunayTriangulation>& delaunay);
};
