#include "rk4_step.h"

void rk4_fixed_step(double& t, vector<Particle*>& particles, double h) {
    /*
      Function to advance set of coupled first-order o.d.e.s by single step
      using fixed step-length fourth-order Runge-Kutta scheme

         x        ... independent variable
         y        ... array of dependent variables
         h        ... fixed step-length

      Requires right-hand side routine

         void rhs_eval (double x, Array<double,1> y, Array<double,1>& dydx)

      which evaluates derivatives of y (w.r.t. x) in array dydx
    */

    // Array y assumed to be of extent n, where n is no. of coupled equations
    int n = particles.size();

    // extract data from particles
    vector<vector<double>> rs(n);
    vector<vector<double>> vs(n);
    vector<double> ms(n);
    vector<double> radii(n);
    for (int i = 0; i < n; i++) {
        Particle p = *particles[i];
        rs[i] = p.get_r();
        vs[i] = p.get_v();
        ms[i] = p.get_m();
        radii[i] = p.get_radius();
    }

    // declare local variables
    vector<vector<double>> acc(n);
    vector<vector<double>> rk1(n), rk2(n), rk3(n), rk4(n), rf(n);
    vector<vector<double>> vk1(n), vk2(n), vk3(n), vk4(n), vf(n);

    // Zeroth intermediate step 
    Octree o0{ rs,ms };
    acc = o0.find_all_accs(rs, ms, radii);
    rk1 = multiply_vector(vs, h);
    vk1 = multiply_vector(acc, h);
    rf = add_vectors(rs, multiply_vector(rk1, 0.5));
    vf = add_vectors(vs, multiply_vector(vk1, 0.5));

    // First intermediate step
    Octree o1{ rf,ms };
    acc = o1.find_all_accs(rf, ms, radii);
    rk2 = multiply_vector(vs, h);
    vk2 = multiply_vector(acc, h);
    rf = add_vectors(rs, multiply_vector(rk2, 0.5));
    vf = add_vectors(vs, multiply_vector(vk2, 0.5));
    
    // Second intermediate step
    Octree o2{ rf, ms };
    acc = o2.find_all_accs(rf, ms, radii);
    rk3 = multiply_vector(vs, h);
    vk3 = multiply_vector(acc, h);
    rf = add_vectors(rs, rk2);
    vf = add_vectors(vs, vk2);

    // Third intermediate step 
    Octree o3{ rf, ms };
    acc = o3.find_all_accs(rf, ms, radii);
    rk4 = multiply_vector(vs, h);
    vk4 = multiply_vector(acc, h);

    // Actual step 
    for (int i = 0; i < n; i++) {
        vector<double> new_r(3);
        vector<double> new_v(3);
        for (int j = 0; j < 3; j++) {
            new_r[j] = rs[i][j] + rk1[i][j] / 6. + rk2[i][j] / 3. + rk3[i][j] / 3. + rk4[i][j] / 6.;
            new_v[j] = vs[i][j] + vk1[i][j] / 6. + vk2[i][j] / 3. + vk3[i][j] / 3. + vk4[i][j] / 6.;
        }
        particles[i]->set_r(new_r);
        particles[i]->set_v(new_v);
    }

    t += h;

    return;
}


void rk4_adaptive_step(double& t, vector<Particle*>& y,
    double& h, double& t_err, double acc,
    double S, int& rept, int maxrept,
    double h_min, double h_max, int flag)
{
    /*
      Function to advance set of coupled first-order o.d.e.s by single step
      using adaptive fourth-order Runge-Kutta scheme

         x       ... independent variable
         y       ... array of dependent variables
         h       ... step-length
         t_err   ... actual truncation error per step
         acc     ... desired truncation error per step
         S       ... step-length cannot change by more than this factor from
                      step to step
         rept    ... number of step recalculations
         maxrept ... maximum allowable number of step recalculations
         h_min   ... minimum allowable step-length
         h_max   ... maximum allowable step-length
         flag    ... controls manner in which truncation error is calculated

      Requires right-hand side routine

            void rhs_eval (double x, Array<double,1> y, Array<double,1>& dydx)

      which evaluates derivatives of y (w.r.t. x) in array dydx.

      Function advances equations by single step whilst attempting to maintain
      constant truncation error per step of acc:

        flag = 0 ... error is absolute
        flag = 1 ... error is relative
        flag = 2 ... error is mixed

      If step-length falls below h_min then routine aborts
    */


    // Array y assumed to be of extent n,  where n is no. of coupled equations
    int n = y.size();

    // Declare local arrays
    vector<Particle*> y0(n), y1(n);

    // Declare repetition counter
    static int count = 0;

    // Save initial data
    double t0 = t;
    y0 = y;

    // Take full step 
    rk4_fixed_step(t, y, h);

    // Save data
    y1 = y;

    // Restore initial data 
    t = t0;
    y = y0;

    // Take two half-steps 
    rk4_fixed_step(t, y, h / 2.);
    rk4_fixed_step(t, y, h / 2.);

    // Calculate truncation error
    t_err = 0.;
    double r_err, v_err, err, err1, err2;
    if (flag == 0)
    {
        // Use absolute truncation error 
        // wont work!
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 3; j++) {
                r_err = fabs(y[i]->get_r()[j] - y1[i]->get_r()[j]);
                v_err = fabs(y[i]->get_v()[j] - y1[i]->get_v()[j]);
                err = (r_err > v_err) ? r_err : v_err;
                t_err = (err > t_err) ? err : t_err;
            }
        }
    }
    else if (flag == 1)
    {
        // Use relative truncation error
        // wont work!
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 3; j++) {
                r_err = fabs((y[i]->get_r()[j] - y1[i]->get_r()[j]) / y[i]->get_r()[j]);
                v_err = fabs((y[i]->get_v()[j] - y1[i]->get_v()[j]) / y[i]->get_v()[j]);
                err = (r_err > v_err) ? r_err : v_err;
                t_err = (err > t_err) ? err : t_err;
            }
        }
    }
    else
    {
        // Use mixed truncation error 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 3; j++) {
                Particle p = *y[i];
                Particle p1 = *y1[i];
                r_err = fabs((p.get_r()[j] - p1.get_r()[j]) / p.get_r()[j]);
                v_err = fabs((p.get_v()[j] - p1.get_v()[j]) / p.get_v()[j]);
                err1 = (r_err > v_err) ? r_err : v_err;

                r_err = fabs(p.get_r()[j] - p1.get_r()[j]);
                v_err = fabs(p.get_v()[j] - p1.get_v()[j]);
                err2 = (r_err > v_err) ? r_err : v_err;

                err = (err1 < err2) ? err1 : err2;
                t_err = (err > t_err) ? err : t_err;
            }
        }
    }

    // Prevent small truncation error from rounding to zero
    if (t_err == 0.) t_err = 1.e-15;

    // Calculate new step-length 
    double h_est = h * pow(fabs(acc / t_err), 0.2);

    // Prevent step-length from changing by more than factor S
    if (h_est / h > S)
        h *= S;
    else if (h_est / h < 1. / S)
        h /= S;
    else
        h = h_est;

    // Prevent step-length from exceeding h_max
    h = (fabs(h) > h_max) ? h_max * h / fabs(h) : h;

    // Abort if step-length falls below h_min
    if (fabs(h) < h_min)
    {
        printf("Error - |h| < hmin\n");
        exit(1);
    }

    // If truncation error acceptable take step 
    if ((t_err <= acc) || (count >= maxrept))
    {
        rept = count;
        count = 0;
    }
    // If truncation error unacceptable repeat step 
    else
    {
        count++;
        t = t0;
        y = y0;
        rk4_adaptive_step(t, y, h, t_err, acc,
            S, rept, maxrept, h_min, h_max, flag);
    }

    y0.clear();

    return;
}