#include "PolylineGenerator.hh"
#include "utils/VoronoiDiagramGenerator.h"
#include <algorithm>
#include <sstream>
#include <cmath>

std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
EmptyPolylineGenerator::generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>> input)
{
  return input;
}

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

std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
HilbertPolylineGenerator::generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>> input)
{
  auto img_size = input.first;
  auto pts = input.second;
  std::sort(pts.begin(), pts.end(), cmp);
  return {img_size, pts};
}

bool MSTPolylineGenerator::cmp::operator()(const sf::Vector2f & a, const sf::Vector2f & b)
{
  if (a.x != b.x)
    return a.x < b.x;
  return a.y < b.y;
}

void MSTPolylineGenerator::GenerateGraph(std::pair<sf::Vector2u, std::vector<sf::Vector2f>> input)
{
  auto img_size = input.first;
  auto pts = input.second;
  int vertices = 0;
  VoronoiDiagramGenerator generator;
  float *vx = new float[pts.size()];
  float *vy = new float[pts.size()];
  for (size_t i=0; i<pts.size(); ++i)
  {
    vx[i] = pts[i].x;
    vy[i] = pts[i].y;
  }
  generator.generateVoronoi(vx, vy, pts.size(), 0, img_size.x-1, 0, img_size.y-1);
  delete[] vx;
  delete[] vy;
  generator.resetIterator();
  sf::Vector2f e1, e2, p1, p2;
  intv.push_back(e1);
  while (generator.getNext(
    e1.x, e1.y, e2.x, e2.y,
    p1.x, p1.y, p2.x, p2.y))
  {
    if (vint[p1]==0)
    {
      vint[p1]=++vertices;
      intv.push_back(p1);
    }
    if (vint[p2]==0)
    {
      vint[p2]=++vertices;
      intv.push_back(p2);
    }
    edges.push_back(sf::Vector2u(vint[p1], vint[p2]));
  }
}

bool MSTPolylineGenerator::EdgCmp::operator()(const sf::Vector2u & a, const sf::Vector2u & b)
{
  auto p=intv[a.x]-intv[a.y], q=intv[b.x]-intv[b.y];
  double A=(double)p.x*p.x+(double)p.y*p.y, B=(double)q.x*q.x+(double)q.y*q.y;
  if (std::abs(A-B)<0.00001)
    return false;
  return A<B;
}

int MSTPolylineGenerator::f(int v)
{
  if (F[v]==v)
    return v;
  return F[v]=f(F[v]);
}

void MSTPolylineGenerator::u(int a, int b)
{
  a=f(a);
  b=f(b);
  F[a]=b;
}

void MSTPolylineGenerator::GenerateMST()
{
  std::ostringstream stream;
  stream <<"There are " <<edges.size() <<" edges and " <<intv.size() <<" vertices";
  logger.log(Logger::Level::Verbose, stream.str().c_str());
  sort(edges.begin(), edges.end(), EdgCmp(intv));
  logger.log(Logger::Level::Debug, "Edges Soreted");
  auto progress = logger.progress(Logger::Level::Verbose,"MST walk through edges", edges.size()); 
  for (size_t i=0; i<edges.size(); ++i)
    F.push_back(i);
  int i=0;
  for (auto it: edges)
  {
    if (f(it.x)!=f(it.y))
    {
      u(it.x, it.y);
      tree.push_back(it);
    }
    if (i%(edges.size()/10) == 0)
      progress.update(i);
    i++;
  }
}

void MSTPolylineGenerator::DFS(int v, std::vector<sf::Vector2f> & list)
{
  visited[v]=1;
  list.push_back(intv[v]);
  for (auto it: graph[v])
    if (!visited[it])
    {
      DFS(it, list);
      list.push_back(intv[v]);
    }
  // list.push_back(intv[v]);
}

std::pair<sf::Vector2u, std::vector<sf::Vector2f>> MSTPolylineGenerator::generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>> input)
{
  auto img_size = input.first;
  auto pts = input.second;
  std::ostringstream stream;
  stream <<"There are " <<pts.size() <<" points ";
  logger.log(Logger::Level::Verbose, stream.str().c_str());
  logger.log(Logger::Level::Debug, "Generating Graph");
  MSTPolylineGenerator::GenerateGraph(input);
  logger.log(Logger::Level::Debug, "Generating MST");
  MSTPolylineGenerator::GenerateMST();
  std::vector<int> empty;
  for (size_t i=0; i<=pts.size(); ++i)
  {
    graph.push_back(empty);
    visited.push_back(0);
  }
  logger.log(Logger::Level::Debug, "After Loop 1");
  for (auto it: tree)
  {
    graph[it.x].push_back(it.y);
    graph[it.y].push_back(it.x);
  }
  logger.log(Logger::Level::Debug, "After Loop 2");
  int b=1;
  for (size_t i=1; i<intv.size(); ++i)
  {
    if (intv[i].x*intv[i].x+intv[i].y*intv[i].y<intv[b].x*intv[b].x+intv[b].y*intv[b].y)
      b=i;
  }
  logger.log(Logger::Level::Debug, "After Loop 3");
  std::vector<sf::Vector2f> output;
  MSTPolylineGenerator::DFS(b, output);
  logger.log(Logger::Level::Debug, "After DFS");
  stream.clear();
  stream <<output.size() <<" edges!";
  logger.log(Logger::Level::Verbose, stream.str().c_str());
  auto w=make_pair(img_size, output);
  return w;
}

