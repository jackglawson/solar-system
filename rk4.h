#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include <iostream>
#include "params.h"
#include "rk4_step.h"
#include "collision.h"
using namespace std;

vector<Particle> walk_to(double t_from, double t_to, vector<Particle> y0);
