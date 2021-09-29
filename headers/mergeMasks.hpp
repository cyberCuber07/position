#ifndef __MERGE_HPP_
#define __MERGE_HPP_

#include "../headers/util.hpp"


// TODO: consider modifying queue to a list --- it's enough...
typedef std::vector<std::queue<int>> MaskGroups; // int coressponds to an index


class MergeMasks {
private:
    Types::vec_i x_1, y_1,
                 x_2, y_2;
    int start_1, end_1,
        start_2, end_2;

    std::vector<int> addPoints (Types::vec_i &, Types::vec_i &, const int &, const int &, const int &);
    void sort_idx (int &, int &);
    void shift_array (Types::vec_i &, Types::vec_i &, const int &, const int &);
    std::queue<int> bfs(Types::vec_b &, const int &, const int &);
    void rps (Types::vec_i &, Types::vec_i &, const int &);
public:
    MergeMasks (const Types::vec_i &, const Types::vec_i &, const Types::vec_i &, const Types::vec_i &, const int &, const int &, const int &, const int &);
    MergeMasks (Types::vec_i &, Types::vec_i &, Types::vec_i &, Types::vec_i &, Types::FixedQueue<float,2>);
    Types::vec_2d_i mergeMasks ();
};


#endif
