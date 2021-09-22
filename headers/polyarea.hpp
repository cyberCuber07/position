#ifndef __POLYAREA_HPP_
#define __POLYAREA_HPP_


#include <vector>
#include "../headers/util.hpp"

// typedef std::vector<int> vec_i;
// typedef std::pair<float,float> pair_2f;

using namespace Types;


class PolyArea{
private:
    vec_i x, y;
    int n_x, n_y;
    float area;
    pair_2f C; // consists of Cx and Cy --- X0 cord center and Y0 cord center
public:   
    PolyArea(const vec_i &, const vec_i &);

    void CalculateArea();
    pair_2f center();
    float count_one_cord (const vec_i &); // helper method for "pair_2f center" method
    void setC (const pair_2f &);
    pair_2f getC();
};

#endif
