#pragma once

#include <vector>
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
};


Simulation run_sim();


