# N-body simulation; Python implementation

Simulates the trajectories of N massive particles under the effects of gravity and inelastic collisions.

All work occurs in Python, apart from calculation of acceleration which occurs in C++.
Run: "setup_list.py install" in cmd to set-up.

Acceleration is calculated by octree method. Collisions are detected with brute force.