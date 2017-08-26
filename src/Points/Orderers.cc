#include "Points/Orderers.hh"
#include <functional>

PointsOrderer::PointsOrderer()
: in(this)
, out(this)
{}


long long Find (int x, int y)
{
  int k=1024*1024;
  long long num=0;
  int p, q, xt, yt;
  while (k>=2)
  {
    k/=2;
    p=(x>k);
    q=(y>k);
    num=num*4+2*p+(p^q);
    if (x>k)
      x-=k;
    if (y>k)
      y-=k;
    if (!q)
    {
      if (!p)
      {
        xt=y;
        yt=x;
      }
      else
      {
        xt=k-y+1;
        yt=k-x+1;
      }
      x=xt;
      y=yt;
    }
  }
  return num;
}


bool cmp (const sf::Vector2f& a, const sf::Vector2f& b)
{
  return Find(a.x, a.y) < Find(b.x, b.y);
}


HilbertPointsOrderer::HilbertPointsOrderer()
{
  name = "HilbertPointsOrderer";
}


void HilbertPointsOrderer::compute()
{
  auto& output = data_hook(out) = in.get_data();
  std::sort(output.pts.begin(), output.pts.end(), cmp);
}


MSTPointsOrderer::MSTPointsOrderer()
: graph(this)
{
  name = "MSTPointsOrderer";
}


void MSTPointsOrderer::compute()
{
  auto& input = in.get_data();
  auto& gr = graph.get_data();
  auto& output = data_hook(out);
  output.size = input.size;
  output.scale = input.scale;

  std::map<sf::Vector2f, sf::Vector2f, Vector2f_cmp> find_map;
  std::map<
    sf::Vector2f,
    std::vector<sf::Vector2f>,
    Vector2f_cmp
  > mst_graph;

  std::function<sf::Vector2f(sf::Vector2f)> fin = [&](sf::Vector2f a)->sf::Vector2f
  {
    if (find_map[a] == a)
      return a;
    return find_map[a] = fin(find_map[a]);
  };

  auto uni = [&](sf::Vector2f a, sf::Vector2f b)->bool
  {
    a = fin(a);
    b = fin(b);
    if (a == b)
      return false;
    find_map[a] = b;
    return true;
  };

  logger.log(Logger::Level::Verbose)<<"MST construction";
  logger.log(Logger::Level::Debug)<<"Sorting edges for Kruskal's algorithm";

  using Edge = std::pair<sf::Vector2f, sf::Vector2f>;

  std::vector<Edge> edges;
  for (auto& node : gr)
    for (auto dst : node.second)
      edges.push_back({node.first, dst});

  auto lensq = [](sf::Vector2f t) -> float {return t.x*t.x+t.y*t.y;};

  std::sort(
    edges.begin(),
    edges.end(),
    [&](Edge a, Edge b)->bool {return lensq(a.first-a.second) < lensq(b.first-b.second);}
  );

  logger.log(Logger::Level::Debug)<<"Find-union initialization";
  for (auto node : input.pts)
    find_map[node] = node;

  logger.log(Logger::Level::Debug)<<"Building actual MST";
  for (auto edge : edges)
    if (uni(edge.first, edge.second))
    {
      mst_graph[edge.first].push_back(edge.second);
      mst_graph[edge.second].push_back(edge.first);
    }

  sf::Vector2f start = input.pts[0];
  float score = lensq(start); // INF

  logger.log(Logger::Level::Debug)<<"Looking for starting node";
  for (auto e : input.pts)
    if (lensq(e) < score)
    {
      start = e;
      score = lensq(e);
    }

  logger.log(Logger::Level::Verbose)<<"Traversing the MST";
  logger.log(Logger::Level::Verbose)<<"Starting node: "<<start.x<<", "<<start.y;
  auto& ret = output.pts;
  std::map<sf::Vector2f, bool, Vector2f_cmp> vis;

  logger.log(Logger::Level::Debug)<<"Building path";
  std::function<void(sf::Vector2f)> dfs = [&](sf::Vector2f w)->void
  {
    vis[w] = true;
    ret.push_back(w);

    for (auto e : mst_graph[w])
      if (!vis[e])
      {
        dfs(e);
        ret.push_back(w);
      }
  };
  dfs(start);
  logger.log(Logger::Level::Verbose)<<"path length is "<<ret.size();
}
