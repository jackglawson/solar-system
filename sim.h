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

class Simulation {
private:
	vector<double> timestamps;
	vector<vector<Particle>> snapshots;

public:
	Simulation(vector<Particle> particles);
	void add_snapshot(double t, vector<Particle> new_rs);
	void print();
	void save(string filename = "simulation.dat");
};


Simulation run_sim(vector<Particle> particles = vector<Particle>{});

