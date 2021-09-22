#include "../headers/polygon.hpp"
#include <vector>
#include <iostream>

void Polygon :: setMaskDistance(const float & val=20) {
    MASK_DISTANCE = val * val;
}


void Polygon :: setPointMaskDistance(const float & val=10) {
    POINT_MASK_DISTANCE = val * val;
}


Polygon :: Polygon (const std::string & file_path) : file_path(file_path) {
    setMaskDistance();
    setPointMaskDistance();
    image = read_image();
    vec_2d_b rect = read_file(file_path); // TODO: consider moving straight into get_all_shapes method call -> when creating this -> shapes
    shapes = get_all_shapes(rect);
    get_all_centers();
}


Polygon :: ~Polygon () {
    for (auto it = vec_polyarea.begin(); it != vec_polyarea.end(); ++it){
        delete *it;
    }
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
        pair_2f pair_tmp = tmp -> center();
        if (pair_tmp.first != 0 && pair_tmp.second != 0){
            tmp -> setC(pair_tmp);
            vec_polyarea.push_back(tmp); // TODO: Get rid of tmp --- memory LEAK !!!
        } else {
            delete tmp;
        }
    }

}


void Polygon :: connectMasks (const int & main_idx, const int & idx) {
}


void Polygon :: rebuildMasks (int & main_idx, const int & idx, int & j, int & n) {
    /*
     * earse vec_polyarea's object with index "idx"
     * change "j" index if main_idx < idx
     * */
    vec_polyarea.erase(vec_polyarea.begin() + idx); // TODO: check if correct
    if (main_idx < idx) {
        --main_idx;
    } else {
        --j; // TODO: check if correct
    }
    --n; // for both cases ??
}


float Polygon :: dis2(const pair_2f & p1, const pair_2f & p2) {
    /*
     * counts dis squared (^2)
     * */
    float tmp_x = p1.first - p2.first,
          tmp_y = p1.second - p2.second;
    return tmp_x * tmp_x + tmp_y * tmp_y;
}


vec_i Polygon :: sameMasks(const int & idx) {
    /*
     * method to find enougly close masks to the given one
     *
     * retult: vector of objects' [vec_polyarea] indexes [int]
     * */
    int n = vec_polyarea.size(), j, i;
    
    auto one_loop_iter = [&](const int & idx, const pair_2f & tmp_i){
        pair_2f tmp = vec_polyarea[idx] -> getC(); // TODO: consider moving to if's dis2 directly
        if (dis2(tmp, tmp_i) < MASK_DISTANCE) {
            /*
             * first: call the connecting two masks method
             *
             * after: call method to earse idx's vec_polyarea object and change index (j in main loop) if necessary
             * */
            connectMasks(i, idx);
            rebuildMasks(i, idx, j, n);
        }
    };

    for (i = 0; i < n; ++i) {
        pair_2f tmp_i = vec_polyarea[i] -> getC();
        for (j = 0; j < i; ++j) one_loop_iter(j, tmp_i);
        for (j = i + 1; j < n; ++j) one_loop_iter(j, tmp_i);
    }
}
