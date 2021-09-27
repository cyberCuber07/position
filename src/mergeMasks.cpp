
#include "../headers/mergeMasks.hpp"


MergeMasks :: MergeMasks (const Types::vec_i & x_1, const Types::vec_i & y_1, const Types::vec_i & x_2, const Types::vec_i & y_2, const int & start_1, const int & end_1, const int & start_2, const int & end_2) : x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2), start_1(start_1), end_1(end_1), start_2(start_2), end_2(end_2) {/* Test constructor --- used in mergeGroup :: getQueue method to find best pairs of indexes */}


MergeMasks :: MergeMasks (Types::vec_i & x_1, Types::vec_i & y_1, Types::vec_i & x_2, Types::vec_i & y_2, const Types::FixedQueue<float,2> & q) : x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2), q(q) {
    /* Final constructor --- modifies the vec_polarea vector!! */
    start_1 = q.front().second.first,
    end_1 = q.front().second.first,
    q.pop();
    start_2 = q.front().second.first,
    end_2 = q.front().second.first;
    q.pop();
}


void MergeMasks :: shift_array (const int & type_idx, const int & start, const int & end, const int & n) {
    /*
     * %%% util method for mergeGroup method %%%
     *
     * chain_idx := {main_idx, idx}
     * */
    
    const int shift_val = end + 1;
 
    // get rid of middle elements --- connecting two chains makes middle parts useless
    const int n_iter = end - start + 1;
    assert (n_iter > 0);

    static auto singleRotation = [&](vec_i & vec){
        std::rotate(vec.begin(), vec.begin() + shift_val, vec.end());
        for (int i = 0; i < n_iter; ++i)
            vec.pop_back();
    };

    // x cord
    singleRotation(vec_polyarea[type_idx] -> x);
    // y cord
    singleRotation(vec_polyarea[type_idx] -> y);
}


void Polygon :: sort_idx (int & idx1, int & idx2) {
    /*
     * %%% util method for mergeGroup method %%%
     *
     * method to make a second index (idx2) bigger
     * */
    if (idx1 > idx2) {
        int tmp = std::move(idx1);
        idx1 = std::move(idx2);
        idx2 = std::move(tmp);
    } else if (idx1 == idx2) {
        // can be assumed that a next point should also be closely enough
        ++idx2;
    }
}


std::vector<int> MergeMasks :: addPoints (Types::vec_i & vec_1, Types::vec_i & vec_2, const int & numOfElements, const int & start, const int & end) {
    return LinSpace::linspace<int,int>(vec_1[start],
                                       vec_2[end],
                                       numOfElements);
    // std::cout << vec.size() << "\n";
    // for (const int & tmp : vec)
    //     std::cout << tmp << " ";
    //     // vec_1.push_back(tmp);
    // std::cout << "END\n";
}


Types::vec_2d_i MergeMasks :: mergeMasks (const int & n1, const int & n2) {
    /*
     * iterate over masks' chains to find two closest pairs of connections (if any)
     *
     * first: find best connection
     *        calculate distances and put them into fixed size queue
     *        in respect to their distance's (get's two smallest pairs)
     *        (iteration over every point cloud's point)
     *
     * second: connect two chains into one
     *
     * third (optional -- here or in outside function) : remove one of the objects
     *
     * TODO: consider changing lamdas to standard methods
     * */

    // first
    // NOTICE: this step has been moved to mergeGroup method !

    // second
    /*
     * 1. shift each chain, so that between index part will be at the end
     * 2. rewrite values from one chain to another
     * 3. delete object of not changed chain
     * */
 
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

    // --- 2. ---
    /* before adding elements from second array (denoted by "idx")
       add connecting elements

       points are added to the first mask (denoted by "main_idx")
       */

    static auto add2Vector = [](const std::vector<int> & vec, std::vector<int> & main_vec){
        for (const int & val : vec) main_vec.push_back(val);
    };

    static auto modVector = [&](const int & start, const int & end, const int & type_idx){
        const int numOfElements = std::max(abs(x_1[start] - x_2[end]),
                                           abs(y_1[start] - y_2[end]));

        std::cout << "NumOfElements: " << numOfElements << "\n";

        std::vector<int> tmp = addPoints(x_1, x_2, numOfElements, start, end);
        add2Vector(tmp, x_1);

        tmp = addPoints(y_1, y_2, numOfElements, start, end);
        add2Vector(tmp, y_1);
    };

    modVector(start_1, end_1, main_idx);
    modVector(start_2, end_2, idx);

    // rewrite elements from chain_2 to chain_1
    int n_chain_2 = x_2.size();
    std::cout << "n_chain_2: " << n_chain_2 << "\n";

    while(n_chain_2--) { // this order connects end to end and head to head
        // x cord
        x_1.push_back(x_2[n_chain_2]);
        // y cord
        y_1.push_back(y_2[n_chain_2]);
    }
    std::cout << y_1.size() << "\n";

    // third
    // TODO: delete chain_2 vec_polyarea object
    // deleted.push_back(tmp_idx); // <- NOTICE: moved to 

    return {x_1, y_1};
}


std::queue<int> MergeMasks :: bfs(vec_b & vis, const int & idx, const int & n) {
    std::queue<int> q;
    q.push(idx);

    static auto one_iter = [&](const int & k){
        if (!vis[k] &&
            dis2(vec_polyarea[k] -> getC(), vec_polyarea[idx] -> getC()) < Polygon::MASK_DISTANCE &&
            vec_polyarea[k] -> x.size() >= 10)
        {
            std::cout << "vec_polyarea[" << k << "]: " << vec_polyarea[k] -> x.size() << "\n";
            q.push(k);
            vis[k] = true;
        }
    };
    
    // one bfs iter
    // ---------------------
    if (!vis[idx]) {
        vis[idx] = true;
        for (int j = 0; j < idx; ++j) one_iter(j);
        for (int j = idx + 1; j < n; ++j) one_iter(j);
    }
    // ---------------------
    return q;
}
