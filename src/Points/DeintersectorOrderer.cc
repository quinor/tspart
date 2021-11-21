#include <Points/Orderers.hh>
#include <set>


static const float EPS = 1e-9;


DeintersectorPointsOrderer::DeintersectorPointsOrderer()
{
  name = "DeintersectorPointsOrderer";
}


void DeintersectorPointsOrderer::_init()
{
  auto& pts = *_pts;
  _idxs.reserve(pts.size());
  for(int i=0; i<(int)_pts->size(); i++)
    _idxs.push_back(i);
  std::sort(_idxs.begin(), _idxs.end(), [&](size_t i, size_t j) {
    return pts[i].x < pts[j].x;
  });
  auto cmp = IdxsComparator(pts);
  _s = std::multiset<size_t, IdxsComparator>(cmp);
}


bool DeintersectorPointsOrderer::_checkIntersection(sf::Vector2f a, sf::Vector2f b,
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
  return _checkIntersection(pts[a], pts[b], pts[c], pts[d]);
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
    _addIntersect(idxA, idx2nd);
    auto it = _s.find(idx2nd);
    if(it != _s.end())
      _s.erase(it);
  }
  if(idx2nd > 0 and pts[idx2nd].x < pts[idx2nd-1].x and
    _checkIntersection(idxA, idxB, idx2nd-1, idx2nd)) {
    _addIntersect(idxA, idx2nd-1);
    auto it = _s.find(idx2nd-1);
    if(it != _s.end())
      _s.erase(it);
  }
}


void DeintersectorPointsOrderer::_handleStartPoint(size_t idxA, size_t idxB)
{
  auto it = _s.insert(idxA);
  if(it != _s.begin()) {
    auto dw = std::prev(it);
    _checkWithStartingAt(idxA, idxB, *dw);
  }
  auto up = std::next(it);
  if(up != _s.end()) {
    _checkWithStartingAt(idxA, idxB, *up);
  }
}


void DeintersectorPointsOrderer::_handleEndPoint(size_t idxA, size_t idxB)
{
  auto it = _s.find(idxB);
  if(it != _s.begin()) {
    auto up = std::next(it);
    auto dw = std::prev(it);
    if(up != _s.end()) {
      _checkStartingAt(*dw, *up);
    }
  }
  it = _s.find(idxA);
  if(it != _s.end()) {
    _s.erase(it);
  }
}


void DeintersectorPointsOrderer::_find()
{
  auto& pts = *_pts;
  for(size_t idx: _idxs) {
    if(idx > 0) {
      if(pts[idx-1].x <= pts[idx].x)
          _handleEndPoint(idx-1, idx);
      else
          _handleStartPoint(idx, idx-1);
    }
    if(idx+1 < pts.size()) {
      if(pts[idx].x <= pts[idx+1].x)
          _handleStartPoint(idx, idx+1);
      else
          _handleEndPoint(idx+1, idx);
    }
  }
}


void DeintersectorPointsOrderer::_remove()
{
}


void DeintersectorPointsOrderer::compute()
{
  Polyline& output = data_hook(out) = in.get_data();
  _pts = &output.pts;
  _init();
  _find();
  // _remove();
  // _clear();
}


