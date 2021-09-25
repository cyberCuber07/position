#include "../headers/util.hpp"


// --------------------------------------------------------------------------------------------------------------------------------
// Class: SelfCross // 
SelfCross :: SelfCross (const int & start_1_x, const int & start_1_y,
                        const int & end_1_x, const int & end_1_y,
                        const int & start_2_x, const int & start_2_y,
                        const int & end_2_x, const int & end_2_y)
{
    start_1 = std::make_pair(start_1_x, start_1_y);
    end_1 = std::make_pair(end_1_x, end_1_y);

    start_2 = std::make_pair(start_2_x, start_2_y);
    end_2 = std::make_pair(end_2_x, end_2_y);
}


bool SelfCross :: selfIntersect () {
    return ccw(start_1, start_2, end_2) != ccw(end_1, start_2, end_2) &&
           ccw(start_1, end_1, start_2) != ccw(start_1, end_1, end_2);
}


bool SelfCross :: ccw (const Types::pair_2i & pa, const Types::pair_2i & pb, const Types::pair_2i & pc) {
    return (pc.second - pa.second) * (pb.first - pa.first) > (pb.second - pa.second) * (pc.first - pa.first);
}



// --------------------------------------------------------------------------------------------------------------------------------

float dis2(const Types::pair_2f & p1, const Types::pair_2f & p2) {
    /*
     * counts dis squared (^2)
     * */
    float tmp_x = p1.first - p2.first,
          tmp_y = p1.second - p2.second;
    return tmp_x * tmp_x + tmp_y * tmp_y;
}


float dis2(const int & tmp_x, const int & tmp_y) {
    /*
     * counts dis squared (^2)
     * */
    return tmp_x * tmp_x + tmp_y * tmp_y;
}
