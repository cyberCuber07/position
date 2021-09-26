#include <iostream>
#include "headers/util.hpp"

template <typename T>
void print_vec(const std::vector<std::vector<T>> & vec) {
    for (const std::vector<T> & v : vec) {
        for (const T & tmp : v) std::cout << tmp << " ";
        std::cout << "\n";
    }
}


int main()
{
    std::vector<double> start = {1, -2}, end = {10, 15};
    std::vector<std::vector<int>> nums = LinSpace::linspaceND<int, double,2>(start, end, 123);

    print_vec(nums);
    std::cout << nums.size() << "," << nums[0].size() << "\n";
}
