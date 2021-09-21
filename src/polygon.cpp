#include "../headers/polygon.hpp"


Polygon :: Polygon (const std::string & file_path) : file_path(file_path) {
    image = read_image();
    vec_2d_b rect = read_file(file_path);
    shapes = get_all_shapes(rect);
}


cv::Mat Polygon :: read_image() {
    return cv::imread(file_path);
}


vec_p2f Polygon :: get_all_centers() {
    int n_shapes = shapes.size();
    vec_p2f Cs (n_shapes);
    for (int k = 0; k < n_shapes; ++k){
        int n = shapes[k].size();
        vec_i x (n), y (n);
        for (int i = 0; i < n; ++i) {
            x[i] = shapes[k][i].first;
            y[i] = shapes[k][i].second;
        }
        PolyArea * polyarea = new PolyArea(x, y);
        Cs[k] = polyarea -> center();
        delete polyarea;
    }

    return Cs;
}
