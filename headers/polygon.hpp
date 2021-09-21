#ifndef __POLYGON_HPP_
#define __POLYGON_HPP_


#include <vector>
#include <string>
#include <opencv2/highgui.hpp>
#include "shapes.hpp"
#include "polyarea.hpp"

typedef std::vector<std::vector<bool>> vec_2d_b;

/*
 * this module reads an image
 * and proceses it using shapes.hpp and polyarea.hpp modules
 *
 * result: center of a mask
 * */


class Polygon{
private:
    vec_2d_b img;
    std::string file_path;
    cv::Mat image;
    vec_2d_p2i shapes;

    cv::Mat read_image();
public:
    Polygon (const std::string &);
    vec_p2f get_all_centers();   
};

#endif
