#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Particle.h"
#include "functions.h"
using namespace std;


void do_collisions(vector<Particle>& particles);
