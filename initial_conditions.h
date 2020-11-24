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


vector<Particle> generate_ics();
