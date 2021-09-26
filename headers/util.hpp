#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <algorithm>
#include <vector>
#include <queue>
#include <deque>

namespace Types{

    // TODO: consider modifying some typedefs (all?) to template aliases..
    typedef std::vector<std::pair<double,double>> vec_p2d;
    typedef std::pair<int,int> pair_2i;
    typedef std::vector<std::vector<bool>> vec_2d_b;
    typedef std::vector<std::vector<std::pair<int,int>>> vec_2d_p2i;
    typedef std::vector<std::vector<int>> vec_2d_i;
    typedef std::vector<std::pair<int,int>> vec_p2i;
    typedef std::vector<std::pair<float,float>> vec_p2f;
    typedef std::vector<std::vector<bool>> vec_2d_b;
    typedef std::vector<int> vec_i;
    typedef std::pair<float,float> pair_2f;
    typedef std::pair<float, std::pair<int,int>> DisIdxs;
    /*
     * in DisIdxs:
     *      float              : distance
     *      std::pair<int,int> : (chain_1_idx, chain_2_idx)
     * */

    template <typename T, int MaxLen, typename Container=std::deque<DisIdxs>>
    class FixedQueue : public std::queue<DisIdxs, Container> {
    public:
 
        void push (const T & value, const int & idx1, const int & idx2) {
            // TODO: add automatic sorting -> check if second is smaller / greater
            //       add complex type :: typedef std::pair<float, std::pair<int,int>> DisIdxs
            if (this -> size() == MaxLen) {
                this -> c.pop_front();
            }
            std::queue<DisIdxs, Container> :: push (std::make_pair(value, std::make_pair(idx1, idx2)));
        }

        FixedQueue<T, MaxLen> createFixedQueue () {
            FixedQueue<T, MaxLen> d;
            for (int i = 0; i < MaxLen; ++i) d.push(1e9, -1, -1);
            return d;
        }
    };
}


class SelfCross {
public:
    /*
     * methods: "ccw" and "selfIntersect" from: https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
     * */
    SelfCross (const int &, const int &, const int &, const int &,
               const int &, const int &, const int &, const int &);
    bool selfIntersect ();
private:
    Types::pair_2i start_1, end_1,
                   start_2, end_2;
    bool ccw (const Types::pair_2i &, const Types::pair_2i &, const Types::pair_2i &);
};



template <typename T>
void switchValues(T & a, T & b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}


float dis2(const Types::pair_2f &, const Types::pair_2f &);

float dis2(const int &, const int &);


namespace LinSpace {
    // code from: https://stackoverflow.com/questions/27028226/python-linspace-in-c
    template <typename T>
    std::vector<double> linspace (T __startIn, T __endIn, int __numIn) {
        
        std::vector<double> __nums;

        const double __start = static_cast<double>(__startIn);
        const double __end = static_cast<double>(__endIn);

        if (__numIn == 0) return __nums;
        if (__startIn == __endIn) return {__start};

        __nums.assign(__numIn, 0);

        const double __delta = (__end - __start) / (static_cast<double>(__numIn) - 1);

        for (int __i = 0; __i < __numIn; ++__i)
            __nums[__i] = __start + __i * __delta;

        return __nums;
    }


    template <typename K>
    bool operator == (std::vector<K> vec_1, std::vector<K> vec_2) {
        for (int i = 0; i < vec_1.size(); ++i)
            if (vec_1[i] != vec_2[i]) return false;
        return true;
    }

    /* extension of "std::generate" to generating 2D vectors counting one element in each row --- initialization */
    template <class ForwardIt, class Generator>
    constexpr ForwardIt generate_ND (ForwardIt startIt, ForwardIt endIt, Generator gen) {
        while (startIt != endIt) {
            *startIt++ = {gen()};
        }
        return startIt;
    }


    template <typename K, typename T, int Dim>
    std::vector<std::vector<K>> linspaceND (std::vector<T> __startIn, std::vector<T> __endIn, int __numIn) {

        std::vector<std::vector<K>> __numsND;

        if (__numIn == 0) return __numsND;
        if (__startIn == __endIn) {
            __numsND.assign(Dim, std::vector<K>(0));
            generate_ND(__numsND.begin(), __numsND.end(), [&](){
                    static int i = 0;
                    return static_cast<K>(__startIn[i++]);
                    });
            return __numsND;
        }

        __numsND.assign(Dim, std::vector<K>(__numIn, 0));

        for (int i = 0; i < Dim; ++i) {
            const K __start = static_cast<K>(__startIn[i]);
            const K __end = static_cast<K>(__endIn[i]);
            const double __delta = (__end - __start) / (static_cast<double>(__numIn) - 1); // double for "delta" is minimum!!!
            for (int j = 0; j < __numIn; ++j)
                __numsND[i][j] = __start + __delta * j; // this implementation makes only one point of "__end" value in each "i" subvector
        }

        return __numsND;
    }
}


#endif
