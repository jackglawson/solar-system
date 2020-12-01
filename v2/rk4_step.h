#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif

#include <vector>
#include "Particle.h"
#include "Octree.h"

using namespace std;

void rk4_fixed_step(double& t, vector<Particle*>& particles, double h);
void rk4_adaptive_step(double& t, vector<Particle*>& y, double& h, double& t_err, int& rept);
