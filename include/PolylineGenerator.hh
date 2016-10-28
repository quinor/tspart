#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Config.hh"
#include "utils/Logger.hh"


class PolylineGenerator
{
protected:
  Config& config;
  Logger& logger;
public:
  PolylineGenerator(Config& cfg, Logger& log)
  : config(cfg)
  , logger(log)
  {}
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>) = 0;
};

class EmptyPolylineGenerator : public PolylineGenerator
{
public:
  using PolylineGenerator::PolylineGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>);
};

class HilbertPolylineGenerator : public PolylineGenerator
{
public:
  using PolylineGenerator::PolylineGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>>
    generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>);
};

class MSTPolylineGenerator : public PolylineGenerator
{
public:
  using PolylineGenerator::PolylineGenerator;
  virtual std::pair<sf::Vector2u, std::vector<sf::Vector2f>> generate(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>);

private:
  struct cmp
  {
    bool operator()(const sf::Vector2f &, const sf::Vector2f &);
  };
  struct EdgCmp
  {
    const std::vector<sf::Vector2f> & intv;
    EdgCmp(const std::vector<sf::Vector2f> & intv_)
    : intv(intv_)
    {}
    bool operator()(const sf::Vector2u &, const sf::Vector2u &);
  };

  std::map<sf::Vector2f, int, cmp> vint;
  std::vector<sf::Vector2f> intv;
  std::vector<sf::Vector2u> edges;
  std::vector<int> F;
  std::vector<sf::Vector2u> tree;
  std::vector<std::vector<int> > graph;
  std::vector<bool> visited;

  int f(int);
  void u(int, int);
  void DFS(int, std::vector<sf::Vector2f> &);
  void GenerateGraph(std::pair<sf::Vector2u, std::vector<sf::Vector2f>>);
  void GenerateMST();
};

