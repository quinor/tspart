#include "FunctionAPI/Points.hh"


GrayscaleImageToScalarField& PointsMixin::grayscale_image_to_scalar_field(
  DataPromise<sf::Texture>& data, Param<size_t> scale)
{
  auto ret = new GrayscaleImageToScalarField();
  register_block(ret);
  ret->in.connect(data);
  ret->scale_input.connect(scale.get_input(this));
  return *ret;
}

ColorImageToScalarField& PointsMixin::color_image_to_scalar_field(
  DataPromise<sf::Texture>& data, Param<size_t> scale)
{
  auto ret = new ColorImageToScalarField();
  register_block(ret);
  ret->in.connect(data);
  ret->scale_input.connect(scale.get_input(this));
  return *ret;
}

ScalarFieldMassPrefixSum& PointsMixin::scalar_field_mass_prefix_sum(
  DataPromise<ScalarField<uint8_t>>& data)
{
  auto ret = new ScalarFieldMassPrefixSum();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

ScalarFieldColorPrefixSum& PointsMixin::scalar_field_color_prefix_sum(
  DataPromise<ScalarField<sf::Color>>& data)
{
  auto ret = new ScalarFieldColorPrefixSum();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

PolylinePloterSaver& PointsMixin::polyline_ploter_saver(DataPromise<Polyline>& data, Param<std::string> name)
{
  auto ret = new PolylinePloterSaver();
  register_block(ret);
  ret->in.connect(data);
  ret->filename_input.connect(name.get_input(this));
  return *ret;
}

PolylineGcodeSaver& PointsMixin::polyline_gcode_saver(DataPromise<Polyline>& data, Param<std::string> name)
{
  auto ret = new PolylineGcodeSaver();
  register_block(ret);
  ret->in.connect(data);
  ret->filename_input.connect(name.get_input(this));
  return *ret;
}

PolylineSVGSaver& PointsMixin::polyline_svg_saver(DataPromise<Polyline>& data, Param<std::string> name)
{
  auto ret = new PolylineSVGSaver();
  register_block(ret);
  ret->in.connect(data);
  ret->filename_input.connect(name.get_input(this));
  return *ret;
}

PointsGenerator& PointsMixin::points_generator(
  DataPromise<ScalarField<uint8_t>>& data, Param<size_t> fill)
{
  auto ret = new PointsGenerator();
  register_block(ret);
  ret->in.connect(data);
  ret->fill_input.connect(fill.get_input(this));
  return *ret;
}

PointsVoronoiDelaunay& PointsMixin::points_voronoi_delaunay(DataPromise<Polyline>& data)
{
  auto ret = new PointsVoronoiDelaunay();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

TriangulationToCells& PointsMixin::triangulation_to_cells(DataPromise<Polyline>& pts, DataPromise<Triangulation>& triangles)
{
  auto ret = new TriangulationToCells();
  register_block(ret);
  ret->centres.connect(pts);
  ret->triangles.connect(triangles);
  return *ret;
}


PointsRelaxator& PointsMixin::points_relaxator(
  DataPromise<Polyline>& data,
  DataPromise<Cells>& cells,
  DataPromise<ScalarField<WeightedElement>>& mass_field)
{
  auto ret = new PointsRelaxator();
  register_block(ret);
  ret->in.connect(data);
  ret->voronoi_cells.connect(cells);
  ret->mass_field.connect(mass_field);
  return *ret;
}

PointsColorAverager& PointsMixin::points_color_averager(
  DataPromise<Polyline>& data,
  DataPromise<Cells>& cells,
  DataPromise<ScalarField<WeightedElement>>& color_field)
{
  auto ret = new PointsColorAverager();
  register_block(ret);
  ret->in.connect(data);
  ret->voronoi_cells.connect(cells);
  ret->color_field.connect(color_field);
  return *ret;
}

HilbertPointsOrderer& PointsMixin::hilbert_points_orderer(DataPromise<Polyline>& data)
{
  auto ret = new HilbertPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

NearestNeighbourPointsOrderer& PointsMixin::nearest_neighbour_points_orderer(DataPromise<Polyline>& data)
{
  auto ret = new NearestNeighbourPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

DeintersectorPointsOrderer& PointsMixin::deintersector_points_orderer(DataPromise<Polyline>& data)
{
  auto ret = new DeintersectorPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}


MSTPointsOrderer& PointsMixin::mst_points_orderer(
  DataPromise<Polyline>& data,
  DataPromise<Triangulation>& graph,
  DataPromise<int>& shrink)
{
  auto ret = new MSTPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  ret->graph.connect(graph);
  ret->shrink.connect(shrink);
  return *ret;
}

SkipPointsOrderer& PointsMixin::skip_points_orderer(
  DataPromise<Polyline>& data,
  DataPromise<Triangulation>& graph,
  DataPromise<int>& shrink)
{
  auto ret = new SkipPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  ret->graph.connect(graph);
  ret->shrink.connect(shrink);
  return *ret;
}


PolylineVisualizer& PointsMixin::polyline_visualizer(DataPromise<Polyline>& data)
{
  auto ret = new PolylineVisualizer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

CellsVisualizer& PointsMixin::voronoi_cells_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<Cells>& voronoi)
{
  auto ret = new CellsVisualizer();
  register_block(ret);
  ret->polyline.connect(data);
  ret->cells.connect(voronoi);
  return *ret;
}

TriangulationVisualizer& PointsMixin::delaunay_triangulation_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<Triangulation>& delaunay)
{
  auto ret = new TriangulationVisualizer();
  register_block(ret);
  ret->polyline.connect(data);
  ret->neighbours.connect(delaunay);
  return *ret;
}

PolygonVisualizer& PointsMixin::polygon_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<Cells>& voronoi,
  DataPromise<ColorMapping>& colors,
  Param<float> shrink)
{
  auto ret = new PolygonVisualizer();
  register_block(ret);
  ret->polyline.connect(data);
  ret->cells.connect(voronoi);
  ret->colors.connect(colors);
  ret->shrink.connect(shrink.get_input(this));
  return *ret;
}


DataPromise<Polyline>& PointsMixin::n_voronoi_relaxation(
  DataPromise<Polyline>& data,
  DataPromise<ScalarField<WeightedElement>>& field,
  int n)
{
  if (n == 0)
    return data;
  return n_voronoi_relaxation(
    points_relaxator(data, points_voronoi_delaunay(data).voronoi, field),
    field,
    n-1);
}

DataPromise<Polyline>& PointsMixin::mst_ordering(
  DataPromise<Polyline>& data,
  Param<int> shrink)
{
  return mst_points_orderer(data, points_voronoi_delaunay(data).delaunay, shrink.get_input(this));
}

DataPromise<Polyline>& PointsMixin::skip_ordering(
  DataPromise<Polyline>& data,
  Param<int> shrink)
{
  return skip_points_orderer(data, points_voronoi_delaunay(data).delaunay, shrink.get_input(this));
}
