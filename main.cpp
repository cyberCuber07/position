#include "headers/shapes.hpp"
#include "headers/polygon.hpp"
#include "headers/util.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include "headers/multi.hpp"


void test(Polygon * polygon) {
    int cnt = 0;
    for (auto tmp : polygon -> getVecPolyArea()) {
        std::cout << ++cnt << ": " << tmp.getC().first << " " << tmp.getC().second << "\n";
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

    int nWorkers = 6;
    RunDir runDir(file_path, nWorkers);

    // Polygon * polygon = new Polygon(file_path, 416, 416);

    // polygon -> connectMasks();

    // test(polygon);

    // polygon -> createImage();

    // delete polygon;
}
