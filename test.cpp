#include <iostream>
#include "headers/polyarea.hpp"
#include <algorithm>
#include <numeric>
#include "headers/util.hpp"


int main()
{
    const int n = 10;
    Types::vec_i NUMS(n, 2);
    // std::iota(NUMS.begin(), NUMS.end(), 0);
    auto modVec = [&](Types::vec_i & vec){
        std::generate(vec.begin(), vec.end(), [&](){
                static int i = 0;
                return NUMS[i++];
                });
    };
    Types::vec_i x(n), y(n);
    modVec(x);
    modVec(y);

    auto get_area = [&](){
        PolyArea tmp(x, y);
        return tmp.getArea();
    };

    std::cout << PolyArea(x, y).getArea() << "\n";

    float area = get_area();

    std::cout << area << "\n";
}
