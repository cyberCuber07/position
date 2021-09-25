#ifndef __SHAPES_HPP_
#define __SHAPES_HPP_


#include <queue>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../headers/util.hpp"

/*
 * this module processes given image as 2d matrix
 * and extract from it all segmentation masks
 *
 * to process use:
 *      get_all_shapes
 * to save all masks use:
 *      save2file
 * */

using namespace Types;
using std::vector;
using std::pair;
using std::queue;


void print_vec(const vec_2d_p2i &);

void print_vec(const vec_p2i &);

void print_vec(const vec_p2f &);

template <typename T>
void print_vec(const vector<vector<T>> &);

vec_2d_b read_file(const std::string &);

bool bounds(const int &, const int &, const int &, const int &);

bool onBound(const int &, const int &, const int &, const int &);

bool isInside(const int &, const int &, const vec_2d_b &, const int &, const int &);

vec_p2i get_one_shape(int, int, const vec_2d_b &, vec_2d_b &, const int &, const int &);

vec_2d_p2i get_all_shapes(const vec_2d_b &);

void test(const vec_2d_p2i &, const int &, const int &, const int &);

void save2file(const vec_2d_p2i &, const std::string &);


#endif
