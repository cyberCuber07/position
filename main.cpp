#include "headers/shapes.hpp"
#include "headers/polygon.hpp"
#include <boost/filesystem.hpp>
#include <iostream>


void test(Polygon * polygon) {
    int cnt = 0;
    for (auto tmp : polygon -> getVecPolyArea()) {
        ++cnt;
        std::cout << cnt << ": " << tmp -> getC().first << " " << tmp -> getC().second << "\n";
    }
    std::cout << "\n";
}


int main(int argc, char **argv)
{
    std::string file_path = argv[1];
    if ( ! boost::filesystem::exists(file_path) ){
        std::cout << "Path does not exist!\n";
        return 1;
    }

    Polygon * polygon = new Polygon(file_path, 416, 416);

    polygon -> connectMasks();

    test(polygon);

    polygon -> createImage();

    delete polygon;
}
