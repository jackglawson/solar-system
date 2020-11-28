#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _USE_MATH_DEFINES
#include <vector>
#include <random>
#include <cmath>
#include "Particle.h"
#include "params.h"
using namespace std;


#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif


vector<Particle*> generate_ics();
