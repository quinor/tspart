#include "FunctionAPI/Points.hh"


ImageToScalarField& PointsMixin::image_to_scalar_field(
  DataPromise<sf::Texture>& data, int scale)
{
  auto ret = new ImageToScalarField();
  register_block(ret);
  ret->in.connect(data);
  if (scale != 2)
    ret->scale_manual.set_data(scale);
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

PolylinePloterSaver& PointsMixin::polyline_ploter_saver(DataPromise<Polyline>& data, std::string name)
{
  auto ret = new PolylinePloterSaver();
  register_block(ret);
  ret->in.connect(data);
  if (name != "")
    ret->filename_manual.set_data(name);
  return *ret;
}

PolylineSVGSaver& PointsMixin::polyline_svg_saver(DataPromise<Polyline>& data, std::string name)
{
  auto ret = new PolylineSVGSaver();
  register_block(ret);
  ret->in.connect(data);
  if (name != "")
    ret->filename_manual.set_data(name);
  return *ret;
}

PointsGenerator& PointsMixin::points_generator(
  DataPromise<ScalarField<uint8_t>>& data, int fill)
{
  auto ret = new PointsGenerator();
  register_block(ret);
  ret->in.connect(data);
  if (fill != 12)
    ret->fill_manual.set_data(fill);
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
  DataPromise<ScalarField<MassElement>>& mass_field)
{
  auto ret = new PointsRelaxator();
  register_block(ret);
  ret->in.connect(data);
  ret->voronoi_cells.connect(cells);
  ret->mass_field.connect(mass_field);
  return *ret;
}

HilbertPointsOrderer& PointsMixin::hilbert_points_orderer(DataPromise<Polyline>& data)
{
  auto ret = new HilbertPointsOrderer();
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

DataPromise<Polyline>& PointsMixin::n_voronoi_relaxation(
  DataPromise<Polyline>& data,
  DataPromise<ScalarField<MassElement>>& field,
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
