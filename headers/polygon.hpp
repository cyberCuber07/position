#ifndef __POLYGON_HPP_
#define __POLYGON_HPP_


#include <string>
#include <opencv2/highgui.hpp> // TODO: for now have been redundant
#include "../headers/shapes.hpp"
#include "../headers/polyarea.hpp"
#include "../headers/util.hpp"

/*
 * this module reads an image
 * and proceses it using shapes.hpp and polyarea.hpp modules
 *
 * stores all separate masks as PolyArea objects
 * also enable to give all connections between them,
 * that is check if they and which ones are enoughly
 * close each other and connect them
 * */

using namespace Types;

typedef std::vector<PolyArea> vec_PolyArea;


class Polygon{
private:
    vec_2d_b img;
    std::string file_path;
    cv::Mat image;
    vec_2d_p2i shapes;
    vec_PolyArea vec_polyarea;
    vec_i deleted;
    int height, width;

    cv::Mat read_image();
    void setMaskDistance(const float &);
    void setPointMaskDistance(const float &);
    std::queue<int> bfs(Types::vec_b &, const int &, const int &);
public:
    float MASK_DISTANCE;
    float POINT_MASK_DISTANCE;
    Polygon (const std::string &, const int &, const int &);
    ~ Polygon();
    void get_all_centers();
    vec_i sameMasks(const int &);
    void connectMasks ();
    vec_PolyArea getVecPolyArea();
    void updatePolyArea();
    void createImage();
};

#endif
