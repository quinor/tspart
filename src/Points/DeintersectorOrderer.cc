#include <Points/Orderers.hh>
#include <exception>
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
  for(size_t i = 0; i < pts.size(); i++)
    _idxs.push_back(i);
  std::sort(_idxs.begin(), _idxs.end(), [&](size_t i, size_t j) {
    return pts[i].x < pts[j].x;
  });
  auto cmp = IdxsComparator(pts);
  _active = std::multiset<size_t, IdxsComparator>(cmp);
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


bool DeintersectorPointsOrderer::_checkIntersection(size_t a, size_t b, size_t c, size_t d)
{
  auto& pts = *_pts;
  return isIntersecting(pts[a], pts[b], pts[c], pts[d]);
}


void DeintersectorPointsOrderer::_checkStartingAt(size_t idx1st, size_t idx2nd)
{
  auto& pts = *_pts;
   if(idx1st+1 < _pts->size() and pts[idx1st].x < pts[idx1st+1].x) {
     _checkWithStartingAt(idx1st, idx1st+1, idx2nd);
   }
   if(idx1st > 0 and pts[idx1st].x < pts[idx1st-1].x) {
     _checkWithStartingAt(idx1st, idx1st-1, idx2nd);
   }
}


void DeintersectorPointsOrderer::_checkWithStartingAt(size_t idxA, size_t idxB, size_t idx2nd)
{
  auto& pts = *_pts;
  if(idx2nd + 1 < _pts->size() and pts[idx2nd].x < pts[idx2nd+1].x and
    _checkIntersection(idxA, idxB, idx2nd, idx2nd+1)) {
    _addIntersection(idxA, idx2nd);
    auto it = _active.find(idx2nd);
    if(it != _active.end())
      _active.erase(it);
  }
  if(idx2nd > 0 and pts[idx2nd].x < pts[idx2nd-1].x and
    _checkIntersection(idxA, idxB, idx2nd-1, idx2nd)) {
    _addIntersection(idxA, idx2nd-1);
    auto it = _active.find(idx2nd-1);
    if(it != _active.end())
      _active.erase(it);
  }
}


void DeintersectorPointsOrderer::_handleStartPoint(size_t idxA, size_t idxB)
{
  auto it = _active.insert(idxA);
  if(it != _active.begin()) {
    auto dw = std::prev(it);
    _checkWithStartingAt(idxA, idxB, *dw);
  }
  auto up = std::next(it);
  if(up != _active.end()) {
    _checkWithStartingAt(idxA, idxB, *up);
  }
}


void DeintersectorPointsOrderer::_handleEndPoint(size_t idxA, size_t idxB)
{
  auto it = _active.find(idxB);
  if(it != _active.begin()) {
    auto up = std::next(it);
    auto dw = std::prev(it);
    if(up != _active.end()) {
      _checkStartingAt(*dw, *up);
    }
  }
  it = _active.find(idxA);
  if(it != _active.end()) {
    _active.erase(it);
  }
}


std::vector<std::pair<size_t, size_t>>& DeintersectorPointsOrderer::find(const std::vector<sf::Vector2f>& pts)
{
  _init_structs(pts);
  for(size_t idx: _idxs) {
    if(idx > 0) {
      if(pts[idx-1].x <= pts[idx].x)
          _handleEndPoint(idx-1, idx);
      else
          _handleStartPoint(idx-1, idx);
    }
    if(idx+1 < pts.size()) {
      if(pts[idx].x <= pts[idx+1].x)
          _handleStartPoint(idx, idx+1);
      else
          _handleEndPoint(idx, idx+1);
    }
  }
  return _intersects;
}

void DeintersectorPointsOrderer::_repin(long a1, long a2, long b1, long b2)
{
  if(_swaper[a1].contains(a2) && _swaper[a2].contains(a1) &&
     _swaper[b1].contains(b2) && _swaper[b2].contains(b1)) {
    _swaper[a1].replace(a2, b1);
    _swaper[b1].replace(b2, a1);
    _swaper[a2].replace(a1, b2);
    _swaper[b2].replace(b1, a2);
  }
}

void DeintersectorPointsOrderer::remove(std::vector<sf::Vector2f>* ptsPtr)
{
  if (_pts != ptsPtr)
    throw std::invalid_argument("Given pointer differs from one that has been used "
                                "to find brutForceFindIntersections");
  _swaper.reserve(_pts->size());
  auto nPts = _pts->size();
  for (size_t i=0; i<nPts-1; i++) {
    _swaper.emplace_back(i-1, i+1);
  }
  _swaper.emplace_back(nPts-2, -1);

  for (auto& pair: _intersects) {
    _repin(pair.first, pair.first+1,
           pair.second, pair.second+1);
  }

  FindUnion fu(_swaper.size());
  for (size_t i = 0; i < _swaper.size(); i++) {
    if (_swaper[i]._x != -1)
      fu.unify(i, _swaper[i]._x);
    if (_swaper[i]._y != -1)
      fu.unify(i, _swaper[i]._y);
  }
  for (auto& pair : _intersects) {
    if (fu.nUnions() == 1)
        break;
    if (fu.find(pair.first) != fu.find(pair.first+1)) {
      _repin(pair.first, pair.second,
             pair.second+1, pair.first+1);
      fu.unify(pair.first, pair.first+1);
    }
  }

  // std::vector<sf::Vector2f> newPts;
  // newPts.reserve(_pts->size());
  long prev = -1;
  long idx = 0;
  for (size_t i = 0; i < ptsPtr->size(); i++) {
    ptsPtr->at(i) = _pts->at(idx);
    auto tmp = _swaper[idx].next(prev);
    prev = idx;
    idx = tmp;
  }
  if (idx> 0)
    throw std::runtime_error("Something went wront. Last element of `_swaper` structure"
                             "shouldn't point to any other element");
  clear();
}


void DeintersectorPointsOrderer::compute()
{
  Polyline& output = data_hook(out) = in.get_data();
  // _pts = &output.pts;
  // _init();
  find(output.pts);
  remove(&output.pts);
  clear();
}


