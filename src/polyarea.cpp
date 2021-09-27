#include "../headers/polyarea.hpp"


PolyArea :: PolyArea(const vec_i & x, const vec_i & y) : x(x), y(y) {
    n_x = n_y = x.size();
    calculateArea();
    calculateCenter();
}


// TODO: think about a better implementation
PolyArea :: PolyArea(Types::vec_2d_i vec) : x(vec[0]), y(vec[1]) {
    n_x = n_y = x.size();
    calculateArea();
    calculateCenter();
}


void PolyArea :: calculateArea() {
    // area = (x[n_x - 1] * y[0] - y[n_x - 1] * x[0]);
    area = 0;
    for (int i = 0; i < n_x - 1; ++i)
        area += x[i] * y[i + 1] - y[i] * x[i + 1];
    area /= 2.;
}


float PolyArea :: count_one_cord (const vec_i & __x) {
    if (area == 0)
        return 0;
    float sum = 0;
    for (int i = 0; i < n_x - 1; ++i)
        sum += (__x[i] + __x[i + 1]) * (x[i] * y[i + 1] - x[i + 1] * y[i]);
    return sum / 6 / area;
}


void PolyArea :: calculateCenter() {
    C = std::make_pair(count_one_cord(x),
                       count_one_cord(y));
}


pair_2f PolyArea :: getC() {
    return C;
}


float PolyArea :: getArea() {
    return area;
}
