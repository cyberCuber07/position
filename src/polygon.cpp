#include "../headers/polygon.hpp"
#include <iostream>
#include <algorithm> // for "std::rotate"
#include <utility> // for "std::move"
#include <fstream>
#include <string>


/*
 * BUGS:
 *      1. indexes instead of cords in "mergeMasks"
 *      2. give indexes instead of cords in "rotate"
 *
 * Rewrite:
 *      "mergeMasks"
 *      "mergeGroup"
 * */

/*
 * TODO:
 *      Consider modyfing "x" and "y" vector ("vec_i") to vector pair ("vec_pi")
 *      or some kind of easier accessibility container... (enum, vector, etc.)
 * */


void print_vec(const vec_i & vec) {
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";
}


void Polygon :: setMaskDistance(const float & val=200) { /* consider changing --- 
                                                            depending on the size of the images
                                                            (CAUTION:: make sure all image are one size only!!)
                                                          */
    MASK_DISTANCE = val * val;
}


void Polygon :: setPointMaskDistance(const float & val=10) {
    POINT_MASK_DISTANCE = val * val;
}


Polygon :: Polygon (const std::string & file_path, const int & height, const int & width) : file_path(file_path), height(height), width(width) {
    assert (height > 0);
    assert (width > 0);
    setMaskDistance();
    setPointMaskDistance();
    image = read_image();
    vec_2d_b rect = read_file(file_path); // TODO: consider moving straight into get_all_shapes method call -> when creating "this -> shapes"
    shapes = get_all_shapes(rect);
    get_all_centers();
}


Polygon :: ~Polygon () {
    // for (auto it = vec_polyarea.begin(); it != vec_polyarea.end(); ++it){
    //     delete *it;
    // }
    vec_polyarea.clear();
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // is: vec_polyarea.clear() --- enough?!?!!
}

cv::Mat Polygon :: read_image() {
    return cv::imread(file_path);
}


void Polygon :: get_all_centers() {
    int n_shapes = shapes.size();
    vec_p2f Cs;
    for (int k = 0; k < n_shapes; ++k){
        int n = shapes[k].size();
        vec_i x (n), y (n);
        for (int i = 0; i < n; ++i) {
            x[i] = shapes[k][i].first;
            y[i] = shapes[k][i].second;
        }
        PolyArea *tmp  = new PolyArea(x, y);
        pair_2f pair_tmp = tmp -> getC();
        if (pair_tmp.first != 0 && pair_tmp.second != 0){
            vec_polyarea.push_back(std::move(tmp)); // TODO: Get rid of tmp --- memory LEAK !!!
        } else {
            delete tmp;
        }
    }
}


void Polygon :: shift_array (const int & type_idx, const int & start, const int & end, const int & n) {
    /*
     * %%% util method for mergeGroup method %%%
     *
     * chain_idx := {main_idx, idx}
     * */
    
    const int shift_val = end + 1;
 
    // get rid of middle elements --- connecting two chains makes middle parts useless
    int n_iter = end - start + 1;
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


void Polygon :: updateIndex(int & idx) {
    int cnt = 0;
    for (int x : deleted) if (x < idx) ++cnt;
    idx -= cnt;
}


// void Polygon :: addConnection(const int & main_idx, const int & idx,
//                               const int & start_1, const int & end_1,
//                               const int & start_2, const int & end_2) {
//     /*
//      * method to add connection points to second chain in "mergeMasks"
//      *
//      * first: calculate number of points that should be added,
//      *        based on the distance of two chains' connection points
//      *        (for each pair ("start", "end") seperatly)
//      *
//      * second: add points to a second chain (denoted by "idx" variable)
//      * */
//     
//     // -- first --
//     static auto max_val = [&](const int & start, const int & end) {
//         const int n_x = abs(vec_polyarea[main_idx] -> x[start] - vec_polyarea[idx] -> x[end]) - 1,
//                   n_y = abs(vec_polyarea[main_idx] -> y[start] - vec_polyarea[idx] -> y[end]) - 1;
//         if (n_x > n_y && n_x > 3) {
//             const int l_y = n_x / n_y;
//             vec_i x_points (n_x);
//             vec_i y_points (n_x, vec_polyarea[);
//             for (int i = 0; i < 
//         } else if (n_y > n_x & n_y > 3) {
//         } else if (n_x > 3 && n_y > 3){ // "n_x == n_y"
//         }
//     };
//     static auto first = [](const vec_i & cord, const int & start, const int & end, const int & n_points) {
//         // lambda to create vector of points for one coordinate ("x" or "y")
//         // "const vec_i & cord := {x, y}"
//         Types::vec_i points (n_points);
//     };
// 
//     // calculate number of points for each connection ("1" and "2")
//     const int n_1 = max_val(start_1, end_1),
//               n_2 = max_val(start_2, end_2);
// }


void Polygon :: mergeMasks (int & main_idx, int & idx) {
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


    /*
     * STEP 0:
     *          Before preceding to alg,
     *          first preprocess indexes
     * */

    int tmp_idx = idx; // make a copy
    updateIndex(main_idx);
    updateIndex(idx);


    static auto save2file = [](const vec_i & vec, const std::string & path) {
        std::ofstream file (path);
        file << vec.size() << "\n";
        for (const int & x : vec) file << x << " ";
        file.close();
    };

    save2file (vec_polyarea[main_idx] -> x, "data/x");
    save2file (vec_polyarea[main_idx] -> y, "data/y");
    // print_vec(vec_polyarea[main_idx] -> x);
    // std::cout << "----------------------------------------------------------------------------------------\n";
    // print_vec(vec_polyarea[main_idx] -> y);


    Types::FixedQueue<float,2> q = Types::FixedQueue<float,2>().createFixedQueue();

    // first
    const int n1 = vec_polyarea[main_idx] -> x.size(), n2 = vec_polyarea[idx] -> x.size();
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {
            float curr_dis = q.front().first, // get distance value
                  tmp_dis = dis2(vec_polyarea[main_idx] -> x[i] - vec_polyarea[idx] -> x[i],
                                 vec_polyarea[main_idx] -> y[i] - vec_polyarea[idx] -> y[i]);
            if (tmp_dis < curr_dis) {
                q.push(tmp_dis, i, j);
            }
        }
    }

    // second
    /*
     * 1. shift each chain, so that between index part will be at the end
     * 2. rewrite values from one chain to another
     * 3. delete object of not changed chain
     * */
    int start_1 = q.front().second.first,
        start_2 = q.front().second.second;
    q.pop();
    int end_1 = q.front().second.first,
        end_2 = q.front().second.second;
    q.pop();
 
    // first preprocess indexes
    /*
     * getting rid of self intersection
     * NOTICE: this total mask's area may increase!!
     * */
    SelfCross selfCross(vec_polyarea[main_idx] -> x[start_1], vec_polyarea[main_idx] -> y[start_1],
                        vec_polyarea[main_idx] -> x[end_1], vec_polyarea[main_idx] -> y[end_1],
                        vec_polyarea[idx] -> x[start_2], vec_polyarea[idx] -> y[start_2],
                        vec_polyarea[idx] -> x[end_2], vec_polyarea[idx] -> y[end_2]);
    if ( selfCross.selfIntersect() )
    {
        switchValues(end_1, end_2); // NOTICE: switching lines' ends doesn't change chain's order (chain no.1 is still no.1!!)
    }

    // sort_idx(start_1, end_1);
    // sort_idx(start_2, end_2);

    std::cout << "cords: " << start_1 << "," << end_1 << " " << start_2 << "," << end_2 << "\n";

    // --- 1. ---
    shift_array(main_idx, start_1, end_1, n1);
    shift_array(idx, start_2, end_2, n2);

    // --- 2. ---
    // rewrite elements from chain_2 to chain_1
    int n_chain_2 = vec_polyarea[idx] -> x.size();
    std::cout << "n_chain_2: " << n_chain_2 << "\n";
    while(n_chain_2--) { // this order connects end to end and head to head
        // x cord
        vec_polyarea[main_idx] -> x.push_back(vec_polyarea[idx] -> x[n_chain_2]);
        // y cord
        vec_polyarea[main_idx] -> y.push_back(vec_polyarea[idx] -> y[n_chain_2]);
    }
    std::cout << vec_polyarea[main_idx] -> y.size() << "\n";
 
    // third
    // TODO: delete chain_2 vec_polyarea object
    std::cout << "size of vec_polyarea: " << vec_polyarea.size() << "\n";
    vec_polyarea.erase(vec_polyarea.begin() + idx); // TODO: this line causes MEMORY LEAK!!! --- delete the pointer
    deleted.push_back(tmp_idx);
}


// TODO: consider modifying queue to a list --- it's enough...
typedef std::vector<std::queue<int>> MaskGroups; // int coressponds to an index


std::queue<int> Polygon :: bfs(vec_b & vis, const int & idx, const int & n) {
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


// void Polygon :: mergeGroup (std::queue<int> & group) {
//     int idx1 = group.front(), idx2;
//     group.pop();
//     vector<int> used; /* NOTICE
//                          consider: indexes := {1, 2, 3}
//                             1. mergeMasks (1, 2), 
//                             2. erase 1.
//                             indexs = {2, 3} // element's "3" index := 1, not 2
//                             ...
// 
//       example:
//           1 2 3 4 -> I. {1, 2}, II. {2, 3} -> [-1] {1, 2}, III. {3, 4} -> [-2] {1, 2}
//           ------------------------------------------------------------------------------------
//                          since idexes will not necesserily be sorted,
//                          then use a vector / list to store and
//                          check if index in vector should be changed
// 
//                          *** downgrading elements should be in respect to those erased ***
//                        */
// 
//     static auto stepDown = [&](int & idx){
//         /*
//          * lambda used to decrease indexes based on NOTICE to "used" vector
//          * */
//         int cnt = 0;
//         for (int x : used)
//             if (x < idx)
//                 ++cnt;
//         idx -= cnt;
//     };
// 
//     while ( !group.empty() ) {
//         idx2 = group.front();
//         group.pop();
// 
//         // stepDown(idx1);
//         // stepDown(idx2);
//         
//         mergeMasks(idx2, idx1); // this order makes second cloud a new one
//                                 // prevents the queue ordering: connect 1. with 2., [new] 2. with 3., ...
//  
//         std::cout << idx1 << " " << idx2 << "\n";
//  
//         // delete idx1 mask
//         // vec_polyarea.erase(vec_polyarea.begin() + idx1);
// 
//         // next iteration
//         used.push_back(idx1);
//         idx1 = std::move(idx2);
// 
//         // print_vec(vec_polyarea[idx2] -> x);
//     }
// }


void Polygon :: mergeGroup (std::queue<int> & group) {
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
        
        mergeMasks(idx1, idx2);
        
        std::cout << "idx_1_n: " << vec_polyarea[idx1] -> x.size() <<
                   ", idx_2_n: " << vec_polyarea[idx2] -> y.size() << "\n";

        // __________________________________________________________

        idx1 = std::move(idx2);

        std::cout << "\n";
    }
}


void Polygon :: connectMasks () {
    /*
     * method to connect all masks that are in valid distance
     * in respect to each other ("closely related masks")
     *
     * 1. get one group of "closely related masks"
     * (iterate over all left masks using standard bfs approach
     * and store a group in a queue)
     *
     * this creates a: vector of queues
     *
     * 2. for each group merge then using "mergeMasks" method
     *
     * 3. udapte values of vec_polyarea objects
     * */

    MaskGroups maskGroups;
    int n = vec_polyarea.size();
    vec_b vis (n, false);

    // --- 1. ---
    for (int i = 0; i < n; ++i) {
        if (!vis[i]) {
            std::queue<int> tmp = bfs(vis, i, n);
            if (!tmp.empty()) maskGroups.push_back(tmp);
        }
    }

    // --- 2. ---
    std::cout << "Number of masks: " << maskGroups.size() << "\n\n";
    std::cout << "Max number: " << vec_polyarea.size() << "\n";
    int cnt = 0;
    for (auto group : maskGroups) {
        std::cout << "Group's size: " << group.size() << "\n";
        cnt += group.size();
        mergeGroup(group);
        std::cout << "Group's finished\n\n";
    }
    std::cout << cnt << "\n";
    std::cout << "Number of masks: " << vec_polyarea.size() << "\n";

    // --- UPDATE MASKS ---
    updatePolyArea();
}


void Polygon :: updatePolyArea() {
    const int n = vec_polyarea.size();
    for (int i = 0; i < n; ++i)
        vec_polyarea[i] = std::move(new PolyArea(vec_polyarea[i] -> x,
                                                 vec_polyarea[i] -> y));
}


vec_PolyArea Polygon :: getVecPolyArea() {
    return vec_polyarea;
}


void Polygon :: createImage() {
    /*
     * method to create cv::Mat image using vec_polyarea objects
     * */
    cv::Mat image(cv::Size(height, width), CV_8UC3);

    // modify for each chain
    // TODO: add cathing errors!!!
    int n_vec = vec_polyarea.size();
    for (int i = 0; i < n_vec; ++i) {
        int n_elements = vec_polyarea[i] -> x.size();
        const cv::Vec3b color(255 / 6 * (i % 6), 255 / 6 * ((i + 1) % 6), 255 / 6 * ((i + 2) % 6));
        for (int k = 0; k < n_elements; ++k)
            image.at<cv::Vec3b>(vec_polyarea[i] -> x[k],
                                vec_polyarea[i] -> y[k]) = color;
        for (int k = 0; k < 3; ++k)
            for (int j = 0; j < 3; ++j)
                image.at<cv::Vec3b>(vec_polyarea[i] -> getC().first + k,
                                    vec_polyarea[i] -> getC().second + j) = cv::Vec3b(125, 125, 125);
        std::cout << "Chain's size: " << vec_polyarea[i] -> x.size() << "\n";
    }

    cv::imwrite("data/savedImage.jpg", image);
}
