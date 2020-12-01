# N-body simulation; C++ implementation

Simulates the trajectories of N massive particles under the effects of gravity and inelastic collisions.

All work occurs in C++, apart from visualisation which occurs in Python.

Acceleration is calculated by octree method. 
Collisions are detected by "sweep and prune" method.
Adaptive RK4 integrator is self-implemented.
