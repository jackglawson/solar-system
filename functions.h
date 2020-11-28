#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif

#include <cstddef>
#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
#include "params.h"
#include <chrono>
using namespace std;

bool is_in_cube(vector<double> centre, double width, vector<double> r);
vector<vector<double>> cartesian_product(vector<double> xs, vector<double> ys, vector<double> zs);
double dist(vector<double> r1, vector<double> r2);
vector<double> acc(vector<double> r_test, vector<double> r, double m);
vector<double> find_com(vector<vector<double>> rs, vector<double> ms);

vector<double> multiply_vector(vector<double> v, double s);
vector<double> add_vectors(vector<double> v1, vector<double> v2);
vector<vector<double>> multiply_vector(vector<vector<double>> v, double s);
vector<vector<double>> add_vectors(vector<vector<double>> v1, vector<vector<double>> v2);
