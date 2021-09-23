#ifndef __POLYAREA_HPP_
#define __POLYAREA_HPP_


#include <vector>
#include "../headers/util.hpp"

/*
 * operations in this module are based on this webpage: https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
 * */

// typedef std::vector<int> vec_i;
// typedef std::pair<float,float> pair_2f;

using namespace Types;


class PolyArea{
private:
    // vec_i x, y; // chain: X0 cord and Y0 cord
    int n_x, n_y;
    float area;
    pair_2f C; // consists of Cx and Cy --- X0 cord center and Y0 cord center
public:   
    PolyArea(const vec_i &, const vec_i &);
    vec_i x, y; // chain: X0 cord and Y0 cord

    void CalculateArea();
    pair_2f center();
    float count_one_cord (const vec_i &); // helper method for "pair_2f center" method
    void setC (const pair_2f &);
    pair_2f getC();
};

#endif
