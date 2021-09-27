#ifndef __POLYAREA_HPP_
#define __POLYAREA_HPP_


#include "../headers/util.hpp"

/*
 * operations in this module are based on this webpage: https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
 * */


using namespace Types;


class PolyArea{
private:
    int n_x, n_y;
    float area;
    pair_2f C; // consists of Cx and Cy --- X0 cord center and Y0 cord center

    void calculateCenter();
    void calculateArea();
public:   
    PolyArea(const vec_i &, const vec_i &);
    PolyArea(const Types::vec_2d_i &);
    vec_i x, y; // chain: X0 cord and Y0 cord
    vec_p2f cord_x, cord_y; // complementary vectors for "x" and "y"

    float count_one_cord (const vec_i &); // helper method for "pair_2f center" method
    pair_2f getC();
    float getArea();
};

#endif
