#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include <string>
#include <fstream>
#include "params.h"
#include "Particle.h"
#include "initial_conditions.h"
#include "rk4.h"
using namespace std;


void create_out_file();
void write_new_snapshot(double timestamp, vector<Particle*> snapshot);
void run_sim(vector<Particle*> particles = vector<Particle*>{});

