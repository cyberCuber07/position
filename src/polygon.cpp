#include "../headers/polygon.hpp"
#include <vector>
#include <iostream>

Polygon :: Polygon (const std::string & file_path) : file_path(file_path) {
    image = read_image();
    vec_2d_b rect = read_file(file_path);
    shapes = get_all_shapes(rect);
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


vec_p2f Polygon :: get_all_centers() {
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
            Cs.push_back(pair_tmp);
            vec_polyarea.push_back(tmp); // TODO: Get rid of tmp --- memory LEAK !!!
        } else {
            delete tmp;
        }
    }

    return Cs;
}
