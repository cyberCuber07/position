
#include "../headers/mergeGroup.hpp"


MergeGroup ::MergeGroup (const vec_PolyArea & vec_polyarea) : vec_polyarea(vec_polyarea) {}


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
 

    int start_1 = q.front().second.first,
        end_1 = q.front().second.second;
    q.pop();
    int start_2 = q.front().second.first,
        end_2 = q.front().second.second;
    q.pop();
 
    // first, preprocess indexes
    /*
     * getting rid of self intersection
     * NOTICE: this total mask's area may increase!!
     * */
    SelfCross selfCross(x_1[start_1], y_1[start_1],
                        x_1[end_1],   y_1[end_1],
                        x_2[start_2], y_2[start_2],
                        x_2[end_2],   y_2[end_2]);
    if ( selfCross.selfIntersect() )
    {
        switchValues(end_1, end_2); // NOTICE: switching lines' ends doesn't change chain's order (chain no.1 is still no.1!!)
    }

    std::cout << "cords: " << start_1 << "," << end_1 << " " << start_2 << "," << end_2 << "\n";

    // --- 1. ---
    shift_array(main_idx, start_1, start_2, n1);
    shift_array(idx, end_1, end_2, n2);
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
        int idx1_tmp = idx1, idx2_tmp = idx2;
        preprocessIndexes(idx1, idx2, n1, n2);   
        Types::FixedQueue<float,2> q = getQueue(idx1, idx2, n1, n2);

        MergeMasks(vec_polyarea[idx1_tmp] -> x,
                   vec_polyarea[idx1_tmp] -> y,
                   vec_polyarea[idx2_tmp] -> x,
                   vec_polyarea[idx2_tmp] -> y).mergeMasks(n1, n2, q);

        std::cout << "size of vec_polyarea: " << vec_polyarea.size() << "\n";
        vec_polyarea.erase(vec_polyarea.begin() + idx2); // TODO: this line causes MEMORY LEAK!!! --- delete the pointer

        deleted.push_back(std::move(idx2_tmp));
 

        std::cout << "idx_1_n: " << vec_polyarea[idx1] -> x.size() <<
                   ", idx_2_n: " << vec_polyarea[idx2] -> y.size() << "\n";

        // __________________________________________________________

        idx1 = std::move(idx2);

        std::cout << "\n";
    }
}
