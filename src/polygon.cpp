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


void Polygon :: setMaskDistance(const float & val=100) { /* consider changing --- 
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


void Polygon :: updateIndex(int & idx) {
    int cnt = 0;
    for (int x : deleted) if (x < idx) ++cnt;
    idx -= cnt;
}


Types::FixedQueue<float,2> Polygon :: getQueue(const int & main_idx, const int & idx, const int & n1, const int & n2) {
    Types::FixedQueue<float,2> q = Types::FixedQueue<float,2>().createFixedQueue();

    for (int start = 0; start < n1; ++start) {
        for (int end = 0; end < n2; ++end) {
            float curr_dis = q.front().first, // get distance value
                  tmp_dis = dis2(vec_polyarea[main_idx] -> x[start] - vec_polyarea[idx] -> x[end],
                                 vec_polyarea[main_idx] -> y[start] - vec_polyarea[idx] -> y[end]);
            if (tmp_dis < curr_dis) { // add area check -> MAXIMIZE IT!!!
                q.push(tmp_dis, start, end);
            }
        }
    }
    return q;
}


std::vector<int> Polygon :: addPoints (Types::vec_i & vec_1, Types::vec_i & vec_2, const int & numOfElements, const int & start, const int & end) {
    return LinSpace::linspace<int,int>(vec_1[start],
                                       vec_2[end],
                                       numOfElements);
    // std::cout << vec.size() << "\n";
    // for (const int & tmp : vec)
    //     std::cout << tmp << " ";
    //     // vec_1.push_back(tmp);
    // std::cout << "END\n";
}


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


    const int n1 = vec_polyarea[main_idx] -> x.size(), n2 = vec_polyarea[idx] -> x.size();

    // first
    Types::FixedQueue<float,2> q = getQueue(main_idx, idx, n1, n2);

    // second
    /*
     * 1. shift each chain, so that between index part will be at the end
     * 2. rewrite values from one chain to another
     * 3. delete object of not changed chain
     * */
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
    SelfCross selfCross(vec_polyarea[main_idx] -> x[start_1], vec_polyarea[main_idx] -> y[start_1],
                        vec_polyarea[idx] -> x[end_1], vec_polyarea[idx] -> y[end_1],
                        vec_polyarea[main_idx] -> x[start_2], vec_polyarea[main_idx] -> y[start_2],
                        vec_polyarea[idx] -> x[end_2], vec_polyarea[idx] -> y[end_2]);
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
        const int numOfElements = std::max(abs(vec_polyarea[main_idx] -> x[start] - vec_polyarea[idx] -> x[end]),
                                           abs(vec_polyarea[main_idx] -> y[start] - vec_polyarea[idx] -> y[end]));

        std::cout << "NumOfElements: " << numOfElements << "\n";

        std::vector<int> tmp = addPoints(vec_polyarea[main_idx] -> x, vec_polyarea[idx] -> x, numOfElements, start, end);
        add2Vector(tmp, vec_polyarea[main_idx] -> x);

        tmp = addPoints(vec_polyarea[main_idx] -> y, vec_polyarea[idx] -> y, numOfElements, start, end);
        add2Vector(tmp, vec_polyarea[main_idx] -> y);
    };

    modVector(start_1, end_1, main_idx);
    modVector(start_2, end_2, idx);

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
