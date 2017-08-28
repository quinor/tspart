#include "FunctionAPI/Points.hh"


ImageToScalarField& PointsMixin::image_to_scalar_field(
  DataPromise<sf::Texture>& data, int scale)
{
  auto ret = new ImageToScalarField();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (scale != 0)
    ret->scale_manual.set_data(scale);
  return *ret;
}

ScalarFieldMassPrefixSum& PointsMixin::scalar_field_mass_prefix_sum(
  DataPromise<ScalarField<uint8_t>>& data)
{
  auto ret = new ScalarFieldMassPrefixSum();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

PointsGenerator& PointsMixin::points_generator(
  DataPromise<ScalarField<uint8_t>>& data, int fill)
{
  auto ret = new PointsGenerator();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (fill != 0)
    ret->fill_manual.set_data(fill);
  return *ret;
}

PointsVoronoiDelaunay& PointsMixin::points_voronoi_delaunay(DataPromise<Polyline>& data)
{
  auto ret = new PointsVoronoiDelaunay();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

PointsRelaxator& PointsMixin::points_relaxator(
  DataPromise<Polyline>& data,
  DataPromise<VoronoiCells>& cells,
  DataPromise<ScalarField<MassElement>>& mass_field)
{
  auto ret = new PointsRelaxator();
  blocks.push_back(ret);
  ret->in.connect(data);
  ret->voronoi_cells.connect(cells);
  ret->mass_field.connect(mass_field);
  return *ret;
}

HilbertPointsOrderer& PointsMixin::hilbert_points_orderer(DataPromise<Polyline>& data)
{
  auto ret = new HilbertPointsOrderer();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

MSTPointsOrderer& PointsMixin::mst_points_orderer(
  DataPromise<Polyline>& data,
  DataPromise<DelaunayTriangulation>& graph)
{
  auto ret = new MSTPointsOrderer();
  blocks.push_back(ret);
  ret->in.connect(data);
  ret->graph.connect(graph);
  return *ret;
}

PolylineVisualizer& PointsMixin::polyline_visualizer(DataPromise<Polyline>& data)
{
  auto ret = new PolylineVisualizer();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

VoronoiCellsVisualizer& PointsMixin::voronoi_cells_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<VoronoiCells>& voronoi)
{
  auto ret = new VoronoiCellsVisualizer();
  blocks.push_back(ret);
  ret->polyline.connect(data);
  ret->cells.connect(voronoi);
  return *ret;
}

DelaunayTriangulationVisualizer& PointsMixin::delaunay_triangulation_visualizer(
  DataPromise<Polyline>& data,
  DataPromise<DelaunayTriangulation>& delaunay)
{
  auto ret = new DelaunayTriangulationVisualizer();
  blocks.push_back(ret);
  ret->polyline.connect(data);
  ret->neighbours.connect(delaunay);
  return *ret;
}
