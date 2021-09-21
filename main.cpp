#include "headers/shapes.hpp"
#include "headers/polygon.hpp"


int main(int argc, char **argv)
{
    std::string file_path = argv[1];
    Polygon * polygon = new Polygon(file_path);

    vec_p2f Cs = polygon -> get_all_centers();

    print_vec(Cs);

    delete polygon;
}
