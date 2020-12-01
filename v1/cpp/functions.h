#pragma once

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
