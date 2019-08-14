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

PointsRelaxator& PointsMixin::points_relaxator(
  DataPromise<Polyline>& data,
  DataPromise<VoronoiCells>& cells,
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
  DataPromise<VoronoiCells>& cells,
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
  DataPromise<DelaunayTriangulation>& graph)
{
  auto ret = new MSTPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  ret->graph.connect(graph);
  return *ret;
}

SkipPointsOrderer& PointsMixin::skip_points_orderer(
  DataPromise<Polyline>& data,
  DataPromise<DelaunayTriangulation>& graph)
{
  auto ret = new SkipPointsOrderer();
  register_block(ret);
  ret->in.connect(data);
  ret->graph.connect(graph);
  return *ret;
}


PolylineVisualizer& PointsMixin::polyline_visualizer(DataPromise<Polyline>& data)
{
  auto ret = new PolylineVisualizer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

VoronoiCellsVisualizer& PointsMixin::voronoi_cells_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<VoronoiCells>& voronoi)
{
  auto ret = new VoronoiCellsVisualizer();
  register_block(ret);
  ret->polyline.connect(data);
  ret->cells.connect(voronoi);
  return *ret;
}

DelaunayTriangulationVisualizer& PointsMixin::delaunay_triangulation_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<DelaunayTriangulation>& delaunay)
{
  auto ret = new DelaunayTriangulationVisualizer();
  register_block(ret);
  ret->polyline.connect(data);
  ret->neighbours.connect(delaunay);
  return *ret;
}

PolygonVisualizer& PointsMixin::polygon_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<VoronoiCells>& voronoi,
  DataPromise<ColorMapping>& colors)
{
  auto ret = new PolygonVisualizer();
  register_block(ret);
  ret->polyline.connect(data);
  ret->cells.connect(voronoi);
  ret->colors.connect(colors);
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

DataPromise<Polyline>& PointsMixin::mst_ordering(DataPromise<Polyline>& data)
{
  return mst_points_orderer(data, points_voronoi_delaunay(data).delaunay);
}

DataPromise<Polyline>& PointsMixin::skip_ordering(DataPromise<Polyline>& data)
{
  return skip_points_orderer(data, points_voronoi_delaunay(data).delaunay);
}
