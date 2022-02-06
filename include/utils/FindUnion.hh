#include <vector>
#include <numeric>


class FindUnion {
  public:
    const size_t size;

    FindUnion(size_t s): size(s),
                         _nUnions(s),
                         _nodes(s),
                         _fathers(s, -1),
                         _ranks(s, 0)  {
        std::iota(_nodes.begin(), _nodes.end(), 0);
    }

    int find(int x) {
        if(_fathers[x] < 0)
            return x;
        _fathers[x] = find(_fathers[x]);
        return _fathers[x];
    }

    int unify(int x, int y) {
        int root_x = find(x);
        int root_y = find(y);
        if(_ranks[root_x] < _ranks[root_y]) {
            _fathers[root_x] = root_y;
            _nUnions--;
            return root_y;
        }
        if(_ranks[root_y] < _ranks[root_x]) {
            _fathers[root_y] = root_x;
            _nUnions--;
            return root_x;
        }
        if(root_x != root_y) {
            _fathers[root_y] = root_x;
            _nUnions--;
            _ranks[root_x]++;
            return root_x;
        }
        return -1;
    }

    size_t nUnions() {
        return _nUnions;
    }
        
  private:
    size_t _nUnions;
    std::vector<int> _nodes;
    std::vector<int> _fathers;
    std::vector<int> _ranks;
};

