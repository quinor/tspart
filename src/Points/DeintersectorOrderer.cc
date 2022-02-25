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
    return pts[i].x < pts[j].x;
  });
  auto cmp = IdxsComparator(pts, &_current_x);
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


bool DeintersectorPointsOrderer::_checkIntersection(size_t idxA, size_t idxB, size_t idxC, size_t idxD)
{
  auto& pts = *_pts;
  bool intersect = isIntersecting(pts[idxA], pts[idxB], pts[idxC], pts[idxD]);
  if(intersect)
    _addIntersection(std::min(idxA, idxB), std::min(idxC, idxD));
  return intersect;
}

void DeintersectorPointsOrderer::_handleStartPoint(size_t idxA, size_t idxB)
{
  _current_x = _pts->at(idxA).x;
  auto it = _active.insert({idxA, idxB}).first;
  auto dw = std::prev(it);
  auto up = std::next(it);
  if(it != _active.begin() &&
      _checkIntersection(idxA, idxB, dw->first, dw->second)) {
    _active.erase(it);
    _active.erase(dw);
  } else if(up != _active.end() &&
      _checkIntersection(idxA, idxB, up->first, up->second)) {
    _active.erase(it);
    _active.erase(up);
  }
}


void DeintersectorPointsOrderer::_handleEndPoint(size_t idxA, size_t idxB)
{
  auto it = _active.find({idxA, idxB});
  if (it == _active.end())
    return;
  if (it != _active.begin()) {
    auto up = std::next(it);
    auto dw = std::prev(it);
    if (up != _active.end()) {
      if(_checkIntersection(dw->first, dw->second, up->first, up->second)) {
        _active.erase(up);
        _active.erase(dw);
      }
    }
  }
  _active.erase(it);
}


std::vector<std::pair<size_t, size_t>>&
DeintersectorPointsOrderer::find(const std::vector<sf::Vector2f>& pts)
{
  _init_structs(pts);
  for (size_t idx: _idxs) {
    if (idx > 0) {
      if (pts[idx-1].x <= pts[idx].x)
        _handleEndPoint(idx-1, idx);
    }
    if (idx+1 < pts.size()) {
      if (pts[idx].x > pts[idx+1].x)
        _handleEndPoint(idx+1, idx);
    }
    if (idx > 0) {
      if (pts[idx-1].x > pts[idx].x) 
        _handleStartPoint(idx, idx-1);
    }
    if (idx+1 < pts.size()) {
      if (pts[idx].x <= pts[idx+1].x)
        _handleStartPoint(idx, idx+1);
    }
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


