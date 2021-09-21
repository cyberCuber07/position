#include "../headers/shapes.hpp"

const vec_2d_i dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};


void print_vec(const vec_2d_p2i &shapes){
    for (vector<pair<int,int>> vec_p : shapes){
        for (pair<int,int> p : vec_p) printf("(%d,%d) ", p.first, p.second);
        std::cout << std::endl;
    }
}


void print_vec(const vec_p2i &shapes){
    for (pair<int,int> p : shapes) printf("(%d,%d) ", p.first, p.second);
        std::cout << std::endl;
}

void print_vec(const vec_p2f &shapes){
    for (pair<float,float> p : shapes) printf("(%f,%f) ", p.first, p.second);
        std::cout << std::endl;
}


template <typename T>
void print_vec(const vector<vector<T>> &shapes){
    for (vector<T> v : shapes){
        for (T val : v) std::cout << val << " ";
        std::cout << std::endl;
    }
}


vec_2d_b read_file(const std::string &file_path){
    std::ifstream file (file_path);

    int row, col;
    file >> row >> col;
    vec_2d_b rect (row, vector<bool>(col));

    for (int i = 0; i < row; ++i){
        for (int j = 0; j < col; ++j){
            int tmp;
            file >> tmp;
            rect[i][j] = static_cast<bool>(tmp);
        }
    }
    return rect;
}


bool bounds(const int &new_row, const int &new_col, const int &row, const int &col) {
    /*
     * this method check's whether the point is inside given bounds
     *
     * arguments:
     *      int row       - point's y cord
     *      int col       - point's x cord
     *      int ROW       - given height
     *      int COL       - given width
     * */
    return !(new_row >= 0 && new_row < row &&
             new_col >= 0 && new_col < col);
}


bool onBound(const int &new_row, const int &new_col, const int &row, const int &col) {
    /*
     * this method check's whether the point is on one of given bounds
     *
     * arguments:
     *      int row       - point's y cord
     *      int col       - point's x cord
     *      int ROW       - given height
     *      int COL       - given width
     * */
    return new_row == 0 || new_row == row - 1 ||
           new_col == 0 || new_col == col - 1;
}


bool isInside(const int &row, const int &col, const vec_2d_b &rect, const int &ROW, const int &COL){
    /*
     * this method check's wether a point is inside a mask or not
     *
     * arguments:
     *      int row       - point's y cord
     *      int col       - point's x cord
     *      vec_2d_b rect - image
     *      int ROW       - image's height
     *      int COL       - image's width
     * */
    if (onBound(row, col, ROW, COL))
        return false;
    for (int i = 0; i < 4; ++i){
        int tmp_row = row + dirs[i][0];
        int tmp_col = col + dirs[i][1];
        if (!bounds(tmp_row, tmp_col, ROW, COL) &&
            !rect[tmp_row][tmp_col])
            return false;
    }
    return true;
}


vec_p2i get_one_shape(int row, int col, const vec_2d_b &rect, vec_2d_b &vis, const int &ROW, const int &COL){
    /*
     * this part of code can ommitted when saving info from "splash" function directly
     * instead of saving it as an image
     * */
    queue<pair<int,int>> q;
    q.push({row, col});
    vec_p2i v;

    // standard bfs implementation
    while (!q.empty()){
        pair<int,int> tmp = q.front();
        q.pop();
        int curr_row = tmp.first,
            curr_col = tmp.second;
        if (!vis[curr_row][curr_col]) {
            vis[curr_row][curr_col] = true;
            if (rect[curr_row][curr_col] &&
                !isInside(curr_row, curr_col, rect, ROW, COL))
                v.push_back({curr_row, curr_col});
            if (!rect[curr_row][curr_col])
                break;
            for (int i = 0; i < 4; ++i){
                int new_curr_row = curr_row + dirs[i][0],
                    new_curr_col = curr_col + dirs[i][1];
                if ( !bounds(new_curr_row, new_curr_col, ROW, COL) &&
                     !vis[new_curr_row][new_curr_col] &&
                     rect[new_curr_row][new_curr_col] ) {
                    q.push({new_curr_row, new_curr_col});
                }
            }
        }
    }
    return v;
}


vec_2d_p2i get_all_shapes(const vec_2d_b &rect){
    /*
     * this method is implemented using BFS over the whole matrix
     * (actually bfs here is unefficient ---
     *  though when modifying consider using different approach,
     *  as this one requires: first (1.) saving to masks to an image,
     *  and then - second (2.) - reading the matrix file
     *  and then THIRD (3.) actually processing it ---
     *  so first two steps (1. & 2.) may be omitted)
     * */
    vec_2d_p2i shapes;
    int row = rect.size(), col = rect[0].size();
    vec_2d_b vis (row, vector<bool>(col));
    int ROW = rect.size(),
        COL = rect[0].size();
    
    for (int i = 0; i < row; ++i){
        for (int j = 0; j < col; ++j){
            if (!vis[i][j]){
                vec_p2i v = get_one_shape(i, j, rect, vis, ROW, COL);
                vis[i][j] = true;
                if (v.size() > 0)
                    shapes.push_back(v);
            }
        }
    }
    return shapes;
}


void test(const vec_2d_p2i &shapes, const int &row, const int &col, const int &val=1){
    /*
     * test method
     *
     * TODO: move to different file
     * */
    if (shapes.size() == 0)
        return;
    vec_2d_i new_rect (row, vector<int> (col));

    for (vector<pair<int,int>> vec_p : shapes){
        for (pair<int,int> p : vec_p){
            new_rect[p.first][p.second] = val;
        }
    }
    print_vec(new_rect);
}


void save2file(const vec_2d_p2i &shapes, const std::string &new_file_name="data/shapes.txt"){
    /*
     * method to create file given the shapes
     * */
    std::ofstream file (new_file_name);

    for (vec_p2i vec_p : shapes){
        for (pair<int,int> p : vec_p){
            file << p.first << " " << p.second << " ";
        }
        file << "\n";
    }
}

