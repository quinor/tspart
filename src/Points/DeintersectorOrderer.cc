#include <Points/Orderers.hh>
#include <exception>
#include <map>
#include <utils/FindUnion.hh>
#include <set>
#include <vector>


static const float EPS = 1e-9;


DeintersectorPointsOrderer::DeintersectorPointsOrderer()
{
  name = "DeintersectorPointsOrderer";
}


void DeintersectorPointsOrderer::_init_structs(const std::vector<sf::Vector2f>& pts)
{
  _pts = &pts;
  _idxs.reserve(pts.size());
  for (size_t i = 0; i < pts.size(); i++)
    _idxs.push_back(i);
  std::sort(_idxs.begin(), _idxs.end(), [&](size_t i, size_t j) {
    if (pts[i].x != pts[j].x)
      return pts[i].x < pts[j].x;
    return pts[i].y < pts[j].y;
  });
  auto cmp = IdxsComparator(pts);
  _active = std::set<std::pair<size_t, size_t>, IdxsComparator>(cmp);
}


bool DeintersectorPointsOrderer::isIntersecting(sf::Vector2f a, sf::Vector2f b,
                                                sf::Vector2f c, sf::Vector2f d)
{
  auto cross = [](sf::Vector2f u, sf::Vector2f v) {
                  return u.x*v.y - u.y*v.x; };
  auto x = cross(b-a, b-c);
  auto y = cross(b-a, b-d);
  auto w = cross(d-c, d-a);
  auto z = cross(d-c, d-b);
  return x * y < -EPS \
      && w * z < -EPS;
}


bool DeintersectorPointsOrderer::_handleIntersection(ActiveIterT seg1,
                                                     ActiveIterT seg2)
{
  auto& pts = *_pts;
  bool intersect = isIntersecting(pts[seg1->first], pts[seg1->second],
                                  pts[seg2->first], pts[seg2->second]);
  if (!intersect)
    return false;

  while(intersect) {
    _addIntersection(std::min(seg1->first, seg1->second),
                     std::min(seg2->first, seg2->second));
    auto prev = std::prev(seg1);
    auto next = std::next(seg2);
    intersect = (seg1 != _active.begin()) && next != _active.end();
    _active.erase(seg1);
    _active.erase(seg2);
    if (!intersect)
      break;
    seg1 = prev;
    seg2 = next;
    intersect = isIntersecting(pts[seg1->first], pts[seg1->second],
                               pts[seg2->first], pts[seg2->second]);
  }
  return true;
}

void DeintersectorPointsOrderer::_handleStartPoint(size_t idxA, size_t idxB)
{
  auto it = _active.insert({idxA, idxB}).first;
  if(it != _active.begin()) {
    auto dw = std::prev(it);
    if(_handleIntersection(dw, it))
      return;
  }
  auto up = std::next(it);
  if(up != _active.end())
    _handleIntersection(it, up);
}


void DeintersectorPointsOrderer::_handleEndPoint(size_t idxA, size_t idxB)
{
  auto it = _active.find({idxA, idxB});
  if(it == _active.end())
    return;
  auto dw = std::prev(it);
  auto up = std::next(it);
  if(it != _active.begin() && up != _active.end())
    _handleIntersection(dw, up);
  _active.erase(it);
}


std::vector<std::pair<size_t, size_t>>&
DeintersectorPointsOrderer::find(const std::vector<sf::Vector2f>& pts)
{
  _init_structs(pts);

  auto cmp = [&](size_t i, size_t j) {
    if (pts[i].x != pts[j].x)
      return pts[i].x < pts[j].x;
    return pts[i].y < pts[j].y;
  };

  for (size_t idx: _idxs) {
    if (idx > 0 && cmp(idx-1, idx))
        _handleEndPoint(idx-1, idx);
    if (idx+1 < pts.size() && cmp(idx+1, idx))
        _handleEndPoint(idx+1, idx);
    if (idx > 0 && cmp(idx, idx-1))
        _handleStartPoint(idx, idx-1);
    if (idx+1 < pts.size() && cmp(idx, idx+1))
        _handleStartPoint(idx, idx+1);
  }
  return _intersects;
}

void DeintersectorPointsOrderer::_repin(long a1, long a2, long b1, long b2)
{
  if (_swapper[a1].contains(a2) && _swapper[a2].contains(a1) &&
     _swapper[b1].contains(b2) && _swapper[b2].contains(b1)) {
    _swapper[a1].replace(a2, b1);
    _swapper[b1].replace(b2, a1);
    _swapper[a2].replace(a1, b2);
    _swapper[b2].replace(b1, a2);
  }
}

void DeintersectorPointsOrderer::remove(std::vector<sf::Vector2f>* ptsPtr)
{
  if (_pts != ptsPtr)
    throw std::invalid_argument("Given pointer differs from one that has been used "
                                "to find brutForceFindIntersections");
  _swapper.reserve(_pts->size());
  auto nPts = _pts->size();
  for (size_t i=0; i<nPts-1; i++) {
    _swapper.emplace_back(i-1, i+1);
  }
  _swapper.emplace_back(nPts-2, -1);

  for (auto& pair: _intersects) {
    _repin(pair.first, pair.first+1,
           pair.second, pair.second+1);
  }

  FindUnion fu(_swapper.size());
  for (size_t i = 0; i < _swapper.size(); i++) {
    if (_swapper[i]._x != -1)
      fu.unify(i, _swapper[i]._x);
    if (_swapper[i]._y != -1)
      fu.unify(i, _swapper[i]._y);
  }
  for (auto& pair : _intersects) {
    if (fu.nUnions() == 1)
        break;
    if (fu.find(pair.first) != fu.find(pair.first+1) &&
        fu.find(pair.first) == fu.find(pair.second) &&
        fu.find(pair.first+1) == fu.find(pair.second+1)) {
      _repin(pair.first, pair.second,
             pair.second+1, pair.first+1);
      fu.unify(pair.first, pair.first+1);
    }
  }

  std::vector<sf::Vector2f> newPts(_pts->size());
  long prev = -1;
  long idx = 0;
  for (size_t i = 0; i < ptsPtr->size(); i++) {
    newPts[i] = _pts->at(idx);
    auto tmp = _swapper[idx].next(prev);
    prev = idx;
    idx = tmp;
  }
  if (idx> 0)
    throw std::runtime_error("Something went wront. Last element of `_swapper` structure"
                             "shouldn't point to any other element");
  *ptsPtr = std::move(newPts);
  clear();
}


void DeintersectorPointsOrderer::compute()
{
  Polyline& output = data_hook(out) = in.get_data();
  while (true) {
    size_t nIntersects = find(output.pts).size();
    if (nIntersects == 0)
      break;
    remove(&output.pts);
  }
  clear();
}


