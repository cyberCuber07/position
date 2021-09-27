#ifndef __MERGE_GROUP_
#define __MERGE_GROUP_

#include "../headers/util.hpp"
#include "../headers/polyarea.hpp"


// TODO: consider modifying queue to a list --- it's enough...
typedef std::vector<std::queue<int>> MaskGroups; // int coressponds to an index


class MergeGroup : public MergeMasks {
private:
    vec_PolyArea vec_polyarea; // extern from polygon.hpp
    Types::vec_i deleted;

    void mergeGroup (std::queue<int> &);
    int preprocessIndexes(int &, int &, int &, int &);
public:
    MergeGroup (const vec_PolyArea &);
};


#endif
