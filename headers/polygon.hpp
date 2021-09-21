#ifndef __POLYGON_HPP_
#define __POLYGON_HPP_


#include <vector>
#include <string>
#include <opencv2/highgui.hpp>
#include "shapes.hpp"
#include "polyarea.hpp"

typedef std::vector<std::vector<bool>> vec_2d_b;
typedef std::vector<PolyArea*> vec_PolyArea;

/*
 * this module reads an image
 * and proceses it using shapes.hpp and polyarea.hpp modules
 *
 * stores all separate masks as PolyArea objects
 * also enable to give all connections between them,
 * that is check if they and which ones are enoughly
 * close each other and connect them
 * */


class Polygon{
private:
    vec_2d_b img;
    std::string file_path;
    cv::Mat image;
    vec_2d_p2i shapes;
    vec_PolyArea vec_polyarea;

    cv::Mat read_image();
public:
    Polygon (const std::string &);
    ~ Polygon();
    vec_p2f get_all_centers();
    vec_i sameMasks(const int &);
};

#endif
