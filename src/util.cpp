#include "util.hpp"


SelfCross :: SelfCross (const int & start_1, const int & end_1,
                        const int & start_2, const int & end_2) {
    p1 = std::make_pair{start_1, end_1};
    p2 = std::make_pair{start_2, end_2};
}


bool SelfCross :: selfIntersect (const Types::pair_2i & pa_1, const Types::pair_2i & pa_2,
                    const Types::pair_2i & pb_1, const Types::pair_2i & pb_2) {
    return ccw(pa_1, pb_1, pb_2) != (pa_2, pb_1, pb_2) && (pa_1, pa_2, pb_1) != (pa_1, pa_2, pb_2);
}


bool ccw (const Types::pair_2i & pa, const Types::pair_2i & pb, const Types::pair_2i & pc) {
    return (pc.second - pa.second) * (pb.first - pa.first) > (pb.second - pa.second) * (pc.first - pa.first);
}


template <typename T>
void switchValues(T & a, T & b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}
