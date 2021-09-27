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


void Polygon :: connectMasks () {
    /*
     * --- Runner method ---
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
    // create mergeGroup object
    MergeGroup mergeGroup ();
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
