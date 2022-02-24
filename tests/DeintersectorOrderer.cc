#define CATCH_CONFIG_MAIN

#include <SFML/System/Vector2.hpp>
// #include <Catch2/catch_amalgamated.hpp>
#include <Points/Orderers.hh>
#include <random>


std::set<std::pair<size_t, size_t>> brutForceFindIntersections(const std::vector<sf::Vector2f>& pts)
{
  std::set<std::pair<size_t, size_t>> set;
  for (size_t i=0; i<pts.size()-1; i++) {
    for (size_t j=i+1; j<pts.size()-1; j++) {
      if (DeintersectorPointsOrderer::isIntersecting(pts[i], pts[i+1], pts[j], pts[j+1])) {
        set.insert(std::make_pair(i, j));
      }
    }
  }
  return set;
}


int checkInclusion(std::vector<std::pair<size_t, size_t>> vec,
                   std::set<std::pair<size_t, size_t>> set)
{
  int errs = 0;
  for(auto pair: vec){
    auto it1 = set.find(pair);
    // std::swap(pair.first, pair.second);
    // auto it2 = set.find(pair);
    if (it1 == set.end() /*&& it2 == set.end()*/) {
      errs++;
    }
  }

  return errs;
}


std::vector<sf::Vector2f> getRandomPoints(size_t size,
                                          float minX, float maxX,
                                          float minY, float maxY, int seed = 0)
{
  std::vector<sf::Vector2f> v;
  v.reserve(size);
  std::random_device rd;
  auto en = seed ? std::default_random_engine(seed) : std::default_random_engine{rd()};
  std::uniform_real_distribution<float> distX(minX, maxX);
  std::uniform_real_distribution<float> distY(minY, maxY);
  for(size_t i=0; i<size; i++) {
    v.emplace_back(distX(en), distY(en));
  }
  return v;
}


// std::vector<sf::Vector2f> get_from_stdin()
// {
//   std::vector<sf::Vector2f> v;
//   float x, y;
//   while(std::cin >> x >> y) {
//     v.emplace_back(x, y);
//   }
//   return v;
// }


std::vector<sf::Vector2f> getSimple()
{
  std::vector<sf::Vector2f> v;
  v.emplace_back(1, 1);
  v.emplace_back(4, 1);
  v.emplace_back(3, 0);
  v.emplace_back(2, 2);
  return v;
}


#include <iostream>
#define REQUIRE(x)                              \
do {                                            \
  bool ans = (x);                               \
  if( ans == false) {                           \
    std::cout << "ERR: " << #x << std::endl;    \
  } else {                                      \
    std::cout << "OK" << std::endl;             \
  }                                             \
} while(0)


int main() {

// TEST_CASE("Simple intersecting", "")
{
  auto x1 = sf::Vector2f(1, 1);
  auto x2 = sf::Vector2f(3, 3);
  auto y1 = sf::Vector2f(1, 3);
  auto y2 = sf::Vector2f(3, 1);
  REQUIRE(DeintersectorPointsOrderer::isIntersecting(x1, x2, y1, y2) == true);
  x1 = sf::Vector2f(1, 1);
  x2 = sf::Vector2f(1, 3);
  y1 = sf::Vector2f(3, 1);
  y2 = sf::Vector2f(3, 3);
  REQUIRE(DeintersectorPointsOrderer::isIntersecting(x1, x2, y1, y2) == false);
  x1 = sf::Vector2f(1, 1);
  x2 = sf::Vector2f(2, 3);
  y1 = sf::Vector2f(3, 1);
  y2 = sf::Vector2f(2, 3);
  REQUIRE(DeintersectorPointsOrderer::isIntersecting(x1, x2, y1, y2) == false);
  x1 = sf::Vector2f(1, 1);
  x2 = sf::Vector2f(2, 3);
  y1 = sf::Vector2f(1, 1);
  y2 = sf::Vector2f(3, 2);
  REQUIRE(DeintersectorPointsOrderer::isIntersecting(x1, x2, y1, y2) == false);
  x1 = sf::Vector2f(1, 1);
  x2 = sf::Vector2f(1, 3);
  y1 = sf::Vector2f(1, 1);
  y2 = sf::Vector2f(1, 3);
  REQUIRE(DeintersectorPointsOrderer::isIntersecting(x1, x2, y1, y2) == false);
}

DeintersectorPointsOrderer dpo;

// TEST_CASE("Simple find", "")
{
  auto pts = getSimple();
  auto intersects = dpo.find(pts);
  REQUIRE(intersects.size() == 1);
  REQUIRE(intersects[0].first == 0 && intersects[0].second == 2);
  dpo.clear();
}

// TEST_CASE("Random small find", "")
{
  auto pts = getRandomPoints(15, 0, 20, 0, 20, 1);
  auto intersects = dpo.find(pts);
  auto all_intersects = brutForceFindIntersections(pts);
  REQUIRE(all_intersects.size() > 0 ? intersects.size() > 0 : true);
  REQUIRE(checkInclusion(intersects, all_intersects) == 0);
  dpo.clear();
}

// TEST_CASE("Random big find", "")
{
  auto pts = getRandomPoints(100, 0, 200, 0, 200, 2);
  auto intersects = dpo.find(pts);
  auto all_intersects = brutForceFindIntersections(pts);
  REQUIRE(all_intersects.size() > 0 ? intersects.size() > 0 : true);
  REQUIRE(checkInclusion(intersects, all_intersects) == 0);
  dpo.clear();
}

// TEST_CASE("Simple remove", "")
{
  auto pts = getSimple();
  auto intersects = dpo.find(pts);
  dpo.remove(&pts);
  auto pts_org = getSimple();
  REQUIRE(pts[0] == pts_org[0]
       && pts[1] == pts_org[2]
       && pts[2] == pts_org[1]
       && pts[3] == pts_org[3]);
  dpo.clear();
}

// TEST_CASE("Random remove progress", "")
{
  auto pts = getRandomPoints(20, 0, 20, 0, 20, 3);
  auto intersects0 = dpo.find(pts);
  int real_n_intersects0 = brutForceFindIntersections(pts).size();
  dpo.remove(&pts);
  auto intersects1 = dpo.find(pts);
  int real_n_intersects1 = brutForceFindIntersections(pts).size();
  if (real_n_intersects0 > 0)
    REQUIRE(real_n_intersects1 < real_n_intersects0);
  dpo.clear();
}


// TEST_CASE("Full untangle small", "")
{
  auto pts = getRandomPoints(20, 0, 20, 0, 20, 4);
  size_t nIntersects;
  for (size_t i = 0; i < pts.size(); i++) {
    nIntersects = dpo.find(pts).size();
    if (nIntersects == 0)
      break;
    dpo.remove(&pts);
  }
  REQUIRE(nIntersects == 0);
  int real_n_intersects = brutForceFindIntersections(pts).size();
  REQUIRE(real_n_intersects == 0);
  dpo.clear();
}


// TEST_CASE("Full untangle big", "")
{
  auto pts = getRandomPoints(1000, 0, 20, 0, 20, 5);
  size_t nIntersects;
  for (size_t i = 0; i < pts.size(); i++) {
    nIntersects = dpo.find(pts).size();
    if (nIntersects == 0)
      break;
    dpo.remove(&pts);
  }
  REQUIRE(nIntersects == 0);
  int real_n_intersects = brutForceFindIntersections(pts).size();
  REQUIRE(real_n_intersects == 0);
  dpo.clear();
}


} // int main

