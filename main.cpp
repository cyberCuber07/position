#include "headers/shapes.hpp"
#include "headers/polygon.hpp"
#include <boost/filesystem.hpp>
#include <iostream>


int main(int argc, char **argv)
{
    std::string file_path = argv[1];
    if ( ! boost::filesystem::exists(file_path) ){
        std::cout << "Path does not exist!\n";
        return 1;
    }

    Polygon * polygon = new Polygon(file_path);

    delete polygon;
}
