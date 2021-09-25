#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <vector>
#include <queue>
#include <deque>

namespace Types{

    // TODO: consider modifying some typedefs (all?) to template aliases..
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
     *      std::pair<int,int> : <chain_1_idx, chain_2_idx>
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
        FixedQueue<T, MaxLen> createFixedQueue ();
    };

    template <typename T, int MaxLen, typename Container>
    FixedQueue<T, MaxLen> FixedQueue<T, MaxLen, Container> :: createFixedQueue () {
        FixedQueue<T, MaxLen> d;
        for (int i = 0; i < MaxLen; ++i) d.push(1e9, -1, -1);
        return d;
    }
}


class SelfCross {
public:
    /*
     * methods: "ccw" and "selfIntersect" from: https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
     * */
    SelfCross (const int &, const int &, const int &, const int &);
    bool selfIntersect (const Types::pair_2i &, const Types::pair_2i &,
                        const Types::pair_2i &, const Types::pair_2i &);
private:
    Types::pair_2i p1, p2;
    bool ccw (const Types::pair_2i &, const Types::pair_2i &, const Types::pair_2i &);
};


#endif
