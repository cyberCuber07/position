#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <vector>

namespace Types{

    typedef std::vector<std::vector<bool>> vec_2d_b;
    typedef std::vector<std::vector<std::pair<int,int>>> vec_2d_p2i;
    typedef std::vector<std::vector<int>> vec_2d_i;
    typedef std::vector<std::pair<int,int>> vec_p2i;
    typedef std::vector<std::pair<float,float>> vec_p2f;
    typedef std::vector<std::vector<bool>> vec_2d_b;
    typedef std::vector<int> vec_i;
    typedef std::pair<float,float> pair_2f;

    template <typename T, int MaxLen, typename Container=std::deque<T>>
    class FixedQueue : public std::queue<T, Container> {
    public:
        void push (const T & value) {
            // TODO: add automatic sorting -> check if second is smaller / greater
            //       add complex type :: typedef std::pair<float, std::pair<int,int>> DisIdxs
            if (this -> size() == MaxLen) {
                this -> c.pop_front();
            }
            std::queue<T, Container> :: push (value);
        }
    };

}

#endif
