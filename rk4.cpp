#include "rk4.h"


vector<Particle*> walk_to(double t_from, double t_to, vector<Particle*> y0) {
    double h = p::h_min;
    double t_err = 0;
    int rept = 0;

    double t = t_from;
    vector<Particle*> y = y0;

    while (t < t_to) {
        rk4_adaptive_step(t, y, h, t_err, p::acc, p::S, rept, p::maxrept, p::h_min, p::h_max, p::flag);
        do_collisions(y);
        //cout << "Stepping to t = " << t << "\n";
    }
    //cout << "Stepping back by " << t - t_to << "\n\n";
    rk4_fixed_step(t, y, t_to - t);

    return y;
}
