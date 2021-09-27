#ifndef __MERGE_GROUP_
#define __MERGE_GROUP_

#include "../headers/util.hpp"
#include "../headers/polyarea.hpp"
#include "../headers/extern.hpp"


// TODO: consider modifying queue to a list --- it's enough...
typedef std::vector<std::queue<int>> MaskGroups; // int coressponds to an index
typedef std::vector<PolyArea*> vec_PolyArea;


class MergeGroup {
private:
    vec_PolyArea vec_polyarea; // extern from polygon.hpp
    Types::vec_i deleted;

    void mergeGroup (std::queue<int> &);
    void preprocessIndexes(int &, int &, int &, int &);
    void updateIndex(int &);
    Types::FixedQueue<float,2> getQueue(const int &, const int &, const int &, const int &);
public:
    MergeGroup (const vec_PolyArea &);
};


#endif
