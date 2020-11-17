#pragma once

#include <vector>
#include "Particle.h"
#include "Octree.h"

using namespace std;

void rk4_fixed_step(double& t, vector<Particle>& particles, double h);

void rk4_adaptive_step(double& t, vector<Particle>& y,
    double& h, double& t_err, double acc,
    double S, int& rept, int maxrept,
    double h_min, double h_max, int flag);