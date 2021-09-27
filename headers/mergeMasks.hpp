#ifndef __MERGE_HPP_
#define __MERGE_HPP_

#include "../headers/util.hpp"


// TODO: consider modifying queue to a list --- it's enough...
typedef std::vector<std::queue<int>> MaskGroups; // int coressponds to an index


class MergeMasks {
protected:
    Types::vec_i x_1, y_1,
                 x_2, y_2;
    Types::FixedQueue<float,2> q;
    int start_1, end_1,
        start_2, end_2;

    Types::vec_2d_i mergeMasks (const int &, const int &);
    std::vector<int> addPoints (Types::vec_i &, Types::vec_i &, const int &, const int &, const int &);
    void sort_idx (int &, int &);
    void shift_array (const int &, const int &, const int &, const int &);
    std::queue<int> bfs(vec_b &, const int &, const int &);
public:
    MergeMasks (const Types::vec_i &, const Types::vec_i &, const Types::vec_i &, const Types::vec_i &);
    MergeMasks (Types::vec_i &, Types::vec_i &, Types::vec_i &, Types::vec_i &, const Types::FixedQueue<float,2> &);
};


#endif
