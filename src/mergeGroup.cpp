
#include "../headers/mergeGroup.hpp"


MergeGroup ::MergeGroup (const vec_PolyArea & vec_polyarea) : vec_polyarea(vec_polyarea) {}


void MergeGroup :: updateIndex(int & idx) {
    int cnt = 0;
    for (int x : deleted) if (x < idx) ++cnt;
    idx -= cnt;
}


Types::FixedQueue<float,2> MergeGroup :: getQueue(const int & main_idx, const int & idx, const int & n1, const int & n2) {
    Types::FixedQueue<float,2> q = Types::FixedQueue<float,2>().createFixedQueue();

    const int n1_half = n1 / 2,
              n2_half = n2 / 2;

    for (int start = 0; start < n1_half; ++start) {
        for (int end = 0; end < n2_half; ++end) {
            int start_1, end_1, start_2, end_2;
            static auto calculateStartEnd = [&](){
                start_1 = n1_half - i,
                start_2 = n1_half + i,
                end_1 = n2_half - j,
                end_2 = n2_half + j;
            };
            calculateStartEnd();
 
            float curr_area = q.front().first,
                  tmp_area = PolyArea(
                                     MergeMasks(
                                         vec_polyarea[main_idx] -> x,
                                         vec_polyarea[main_idx] -> y,
                                         vec_polyarea[idx] -> x,
                                         vec_polyarea[idx] -> y,
                                         start_1,
                                         end_1,
                                         start_2,
                                         end_2).mergeMasks(n1, n2)
                                     ).getArea();
            if (tmp_area < curr_area) {
                q.push(tmp_area, start, end);
            }
        }
    }
    return q;
}


void MergeGroup :: preprocessIndexes(int & main_idx, int & idx, int & n1, int & n2) {
    /*
     * STEP 0:
     *          Before preceding to alg,
     *          first preprocess indexes
     * */
    updateIndex(main_idx);
    updateIndex(idx);

    n1 = vec_polyarea[main_idx] -> x.size(),
    n2 = vec_polyarea[idx] -> x.size();
    
    return getQueue(main_idx, idx, n1, n2);
}


void MergeGroup :: mergeGroup (std::queue<int> & group) {
    /*
     * method to iterate over a group and connect all masks to one
     * */
    int idx1 = group.front(), idx2;
    group.pop();
    while (!group.empty()) {
        idx2 = group.front();
        group.pop();

        // std::cout << idx1 << " " << idx2 << "\n";

        /* MAIN CODE */
        // __________________________________________________________
     
        int n1, n2; // number of 
        int tmp_idx = idx2; // make a copy

        q = preprocessIndexes(idx1, idx2, n1, n2);

        MergeMasks(
            vec_polyarea[main_idx] -> x,
            vec_polyarea[main_idx] -> y,
            vec_polyarea[idx] -> x,
            vec_polyarea[idx] -> y,
            q).mergeMasks(n1, n2);

        std::cout << "size of vec_polyarea: " << vec_polyarea.size() << "\n";
        vec_polyarea.erase(vec_polyarea.begin() + idx2); // TODO: this line causes MEMORY LEAK!!! --- delete the pointer

        deleted.push_back(std::move(tmp_idx));
 

        std::cout << "idx_1_n: " << vec_polyarea[idx1] -> x.size() <<
                   ", idx_2_n: " << vec_polyarea[idx2] -> y.size() << "\n";

        // __________________________________________________________

        idx1 = std::move(idx2);

        std::cout << "\n";
    }
}
