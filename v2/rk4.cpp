#include "rk4.h"


vector<Particle*> walk_to(double t_from, double t_to, vector<Particle*> y) {
    double h = p::h_start;
    double t_err = 0;
    int rept = 0;

    double t = t_from;

    while (t < t_to) {
        rk4_adaptive_step(t, y, h, t_err, rept);
        if (p::collisions_on) {do_collisions(y);}
        //cout << "Stepping to t = " << t << "\n";
    }
    //cout << "Stepping back by " << t - t_to << "\n\n";
    rk4_fixed_step(t, y, t_to - t);

    return y;
}
