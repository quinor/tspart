#include "PolylineGenerator.hh"

#include <algorithm>

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
