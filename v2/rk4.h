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
#include <iostream>
#include "params.h"
#include "rk4_step.h"
#include "collision.h"
using namespace std;

vector<Particle*> walk_to(double t_from, double t_to, vector<Particle*> y0);
