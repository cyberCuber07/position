#ifndef __MULTI_HPP_
#define __MULTI_HPP_

#include "../headers/polygon.hpp"


class RunDir {
private:
    int nWorkers, n;
    std::string dir_path;
    std::vector<std::string> paths;
    // static std::vector<std::string> fileEnding;
    /*
     * nWorkers - number of threads
     * n - number of paths
     * paths - all images' paths in provided directory
     * */

    void iterateDir();
    void readDir ();
    std::string get_path(const boost::filesystem::directory_entry & path);
public:
    RunDir (const std::string &, const int &);
    std::vector<std::string> fileEnding;
};


#endif
