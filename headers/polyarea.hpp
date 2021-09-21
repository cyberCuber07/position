#ifndef __POLYAREA_HPP_
#define __POLYAREA_HPP_


#include <vector>

typedef std::vector<int> vec_i;
typedef std::pair<float,float> pair_2f;


class PolyArea{
private:
    vec_i x, y;
    int n_x, n_y;
    float area;
public:   
    PolyArea(const vec_i &, const vec_i &);

    void CalculateArea();
    pair_2f center();
    float count_one_cord (const vec_i &); // helper method for "pair_2f center" method
};

#endif
