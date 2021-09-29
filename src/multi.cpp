
#include "../headers/multi.hpp"
#include <boost/algorithm/string/predicate.hpp>


RunDir :: RunDir (const std::string & path, const int & nWorkers) : dir_path(path), nWorkers(nWorkers) {

    fileEnding = {{".txt"}};

    readDir ();

    for (auto tmp : paths) std::cout << "Final_word: " << tmp << "\n";

    iterateDir();
    n = paths.size();
    std::cout << n << "\n";
}


void RunDir :: iterateDir() {
    const int iter = n / nWorkers;
    for (int i = 0; i < iter; ++i) {
        std::vector<std::thread> ths(nWorkers);
        for (int k = 0; k < nWorkers; ++k) {
            ths[k] = std::thread([&]{
                Polygon(paths[i * iter + k], 416, 416).connectMasks();
                    });
        }
        for (int k = 0; k < nWorkers; ++k)
            ths[k].join();
    }
}


void RunDir :: readDir () {
    boost::filesystem::path p = static_cast<boost::filesystem::path>(dir_path);
    boost::filesystem::directory_iterator it{p};
    while (it != boost::filesystem::directory_iterator{}) {
        std::string tmp_path = get_path(*it);
        if (tmp_path.compare("NULL") != 0)
                paths.push_back(tmp_path);
        ++it;
    }
}


std::string RunDir :: get_path(const boost::filesystem::directory_entry & path) {
    std::stringstream oss;
    oss << path;
    const std::string tmp_path = oss.str();
    std::string final_path = tmp_path.substr(1, tmp_path.size() - 2);
    for (const std::string & word_ending : fileEnding)
        if ( boost::algorithm::ends_with(final_path, word_ending) )
            return final_path;
    return "NULL";
}
