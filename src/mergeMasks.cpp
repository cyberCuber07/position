
#include "../headers/mergeMasks.hpp"
#include <cassert>
#include <fstream>


MergeMasks :: MergeMasks (const Types::vec_i & x_1, const Types::vec_i & y_1, const Types::vec_i & x_2, const Types::vec_i & y_2, const int & start_1, const int & end_1, const int & start_2, const int & end_2) : x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2), start_1(start_1), end_1(end_1), start_2(start_2), end_2(end_2) {/* Test constructor --- used in mergeGroup :: getQueue method to find best pairs of indexes */}


MergeMasks :: MergeMasks (Types::vec_i & x_1, Types::vec_i & y_1, Types::vec_i & x_2, Types::vec_i & y_2, Types::FixedQueue<float,2> q) : x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2) {
    /* Final constructor --- modifies the vec_polarea vector!! */
    const std::vector<int> tmp = q.front().second;
    start_1 = tmp[0],
    end_1 =   tmp[1],
    start_2 = tmp[2],
    end_2 =   tmp[3];
    q.pop();
}


void MergeMasks :: shift_array (Types::vec_i & x, Types::vec_i & y, const int & start, const int & end) {
    /*
     * %%% util method for mergeGroup method %%%
     *
     * chain_idx := {main_idx, idx}
     * */
    
    const int shift_val = end + 1;
 
    // get rid of middle elements --- connecting two chains makes middle parts useless
    int n_iter = abs(end - start) + 1; // TODO: test it..
    assert (n_iter > 0);

    static auto singleRotation = [&](Types::vec_i & vec){
        std::rotate(vec.begin(), vec.begin() + shift_val, vec.end());
        while (n_iter--)
            vec.pop_back();
    };

    // x cord
    singleRotation(x);
    // y cord
    singleRotation(y);
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


void save2file (const Types::vec_i & vec) {
    static int i = 0;
    std::string path = "data/" + std::to_string(i++);
    std::ofstream file (path);
    file << vec.size() << "\n";
    for (const int & x : vec) file << x << " ";
    file << "\n";
    file.close();
}


void MergeMasks :: rps (Types::vec_i & vec_1, Types::vec_i & vec_2, const int & numOfElements) {
    Types::vec_i idx2main = addPoints(vec_1, vec_2, numOfElements, start_2, end_2);
    // reverse
    std::reverse(idx2main.begin(), idx2main.end());
    // push_back
    for (const int & val : idx2main) vec_2.push_back(val);
    // shift
    const int n_rot = vec_2.size(); // test
    std::rotate(vec_2.begin(), vec_2.begin() + n_rot, vec_2.end());
};



Types::vec_2d_i MergeMasks :: mergeMasks () {
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
    if ( SelfCross(x_1[start_1], y_1[start_1],
                   x_1[start_2], y_1[start_2],
                   x_2[end_1],   y_2[end_1],
                   x_2[end_2],   y_2[end_2]).selfIntersect() )
    {
        switchValues(end_1, end_2); // NOTICE: switching lines' ends doesn't change chain's order (chain no.1 is still no.1!!)
    }

    std::cout << "cords: " << start_1 << "," << end_1 << " " << start_2 << "," << end_2 << "\n";

    // --- 1. ---
    shift_array(x_1, y_1, start_1, start_2);
    shift_array(x_2, y_2, end_1, end_2);

    // --- 2. ---
    /* before adding elements from second array (denoted by "idx")
       add connecting elements

       points are added to the first mask (denoted by "main_idx")
       */
 
    
    // modify each array independently
    static auto getNumOfElements = [&](const int & start, const int & end){
        return std::max(abs(x_1[start] - x_2[end]),
                        abs(y_1[start] - y_2[end]));
    };

    int numOfElements = getNumOfElements(start_1, end_1);

    std::cout << "numOfElements: " << numOfElements << "\n";

    Types::vec_i x_1_tmp = x_1;
    Types::vec_i y_1_tmp = y_1;

    // FIRST: simple push_back
    for (const int & val : addPoints(x_1, x_2, numOfElements, start_1, end_1)) x_1.push_back(val);
    for (const int & val : addPoints(y_1, y_2, numOfElements, start_1, end_1)) y_1.push_back(val);

    numOfElements = getNumOfElements(start_2, end_2);
    std::cout << "numOfElements: " << numOfElements << "\n";

    // SECOND: reverse array, then push_back, then shift
    rps(x_2, x_1_tmp, numOfElements);
    rps(y_2, y_1_tmp, numOfElements);

    save2file(x_1);
    save2file(y_1);
    save2file(x_2);
    save2file(y_2);


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
 
    save2file(x_1);
    save2file(x_2);

    return {x_1, y_1};
}
