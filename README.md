# N-body simulation

Simulates the trajectories of N massive particles under the effects of gravity and inelastic collisions.

v1 and v2 are two separate implementations. Both are broadly similar but v1 is mostly Python and v2 is mostly C++.

High level:

- Movement is simulated by adaptive RK4 integration
- Collisions are handled by sweep and prune
- An octree is used to cluster the center of masses, as an approximation to reduce the complexity from N^2 

![Alt Text](https://github.com/jackglawson/solar_system/blob/master/v2/examples/eg1.gif)
