import numpy as np
import pickle
import time

from collision import get_particle_pairs, coalesce, combine_overlapping_particles, CollisionEvent, \
    find_collision_pair_index
from general_functions import find_acc_v, find_particle_radius
from initial_conditions import generate_initial_conditions, give_ics
from my_ivp import solve_ivp
from params import p
from octree_module import find_acc_cpp


class WholeSolution:
    """
    Store the solution in a convenient form
    The position can be accessed by self.rs[time, particle, dimension]
    """

    def __init__(self):
        self.t = np.arange(0, p.tot_t, p.dt)
        self.collision_pairs = []
        self.collision_times = []
        self.ys = []
        print(p.snapshots, p.num_particles, p.num_dimensions)
        self.rs = np.empty((p.snapshots, p.num_particles, p.num_dimensions))
        self.rs[:] = np.nan
        self.iteration = 0
        self.Ns = np.ndarray(p.snapshots)
        self.ms = np.empty((p.snapshots, p.num_particles))
        self.ms[:] = np.nan

    def add_solution_part(self, solution_part, N, ms):
        self.Ns[self.iteration:self.iteration + len(solution_part.t)] = N
        particle_pairs = get_particle_pairs(N)

        collision_pair_index = find_collision_pair_index(solution_part.t_events)
        if collision_pair_index is not None:
            collision_pair = particle_pairs[collision_pair_index]
            collision_time = solution_part.t_events[collision_pair_index][0]
            self.collision_pairs.append(collision_pair)
            self.collision_times.append(collision_time)

        rs = solution_part.y[:int(len(solution_part.y) / 2), :].T.reshape((len(solution_part.t), N, p.num_dimensions))
        self.rs[self.iteration:self.iteration + len(solution_part.t), :N, :] = rs

        self.ys.append(solution_part.y)

        self.ms[self.iteration:self.iteration + len(solution_part.t), :N] = ms

        self.iteration += len(solution_part.t)


def find_acc_cpp_wrapper(rs, ms):
    rs_tuple = tuple(map(tuple, rs))
    ms_tuple = tuple(ms)
    radii_tuple = tuple(find_particle_radius(ms))
    As = find_acc_cpp(rs_tuple, ms_tuple, radii_tuple, (p.qt, p.G))
    As = np.array(As)
    return As


def ode_func(t, y, ms, n):
    """
    Function to be passed into solve_ivp
    :param t: float
    :param y: np.ndarray
        All variables, in form [r1x, r1y, r2x, r2y, r3x, r3y, v1x, v1y, v2x, v2y, v3x, v3y]
    :param ms: list
    :param n: int
        Number of particles
    :return: dydt: np.ndarray
         In form [v1x, v1y, v2x, v2y, v3x, v3y, a1x, a1y, a2x, a2y, a3x, a3y]
    """

    rs = y[:int(len(y) / 2)].reshape((n, p.num_dimensions))
    vs = y[int(len(y) / 2):].reshape((n, p.num_dimensions))

    if p.gravity_on:
        if p.calc_types[p.calc_type] == "py_brute":
            As = find_acc_v(rs, ms, n)
        elif p.calc_types[p.calc_type] == "cpp_octree":
            As = find_acc_cpp_wrapper(rs, ms)
        elif p.calc_types[p.calc_type] == "adaptive":
            if len(ms) < p.use_brute_below:
                print("using brute")
                As = find_acc_v(rs, ms, n)
            else:
                print("ucing octree")
                As = find_acc_cpp_wrapper(rs, ms)
        else:
            raise Exception("bad calc type")
    else:
        As = np.zeros((n, p.num_dimensions))

    dydt = np.concatenate([vs, As]).flatten()

    return dydt


def run_sim(filename='', ics=None):
    start_time = time.time()

    if ics:
        y_0, ms = ics
    elif p.use_given_ics:
        y_0, ms = give_ics()
        assert len(ms) == p.num_particles
    else:
        y_0, ms = generate_initial_conditions()

    whole_solution = WholeSolution()
    snapshot_number = 0
    collision_time = 0

    while True:
        n = len(ms)
        print('Continuing integration at time {}. n = {}. Time elapsed = {}s'.format(round(collision_time, 2), n,
                                                                                     round(time.time() - start_time,
                                                                                           1)))
        t_span = (collision_time, p.tot_t)
        t_eval = np.linspace(snapshot_number * p.dt, p.tot_t, p.snapshots - snapshot_number, endpoint=False)
        particle_pairs = get_particle_pairs(n)
        collision_events = [CollisionEvent(i, j) for i, j in particle_pairs] if p.collisions_on else []

        solution_part = solve_ivp(ode_func, t_span, y_0, args=(ms, n), t_eval=t_eval, events=collision_events,
                                  max_step=p.max_step, vectorized=True, rtol=p.rtol, atol=p.atol)
        whole_solution.add_solution_part(solution_part, n, ms)
        print(solution_part.message)

        collision_pair_index = find_collision_pair_index(solution_part.t_events)
        if collision_pair_index is None:
            break

        collision_pair = particle_pairs[collision_pair_index]
        collision_time = solution_part.t_events[collision_pair_index][0]

        y_final = solution_part.y_events[collision_pair_index][0]
        rs_final = y_final[:int(len(y_final) / 2)].reshape((n, p.num_dimensions))
        vs_final = y_final[int(len(y_final) / 2):].reshape((n, p.num_dimensions))

        rs, vs, ms = coalesce(collision_pair, rs_final, vs_final, ms)
        rs, vs, ms = combine_overlapping_particles(rs, vs, ms)
        y_0 = np.concatenate([rs, vs]).flatten()
        snapshot_number += len(solution_part.t)

        del solution_part

        if filename and p.save_continuously:
            print('Saving to {}'.format(filename))
            with open(filename, 'wb') as f:
                pickle.dump(whole_solution, f)

    time_taken = time.time() - start_time
    print('\n\nTotal time taken = {}s'.format(round(time_taken, 1)))

    if filename:
        print('Saving to {}'.format(filename))
        with open(filename, 'wb') as f:
            pickle.dump(whole_solution, f)

    return whole_solution


def unpickle(filename):
    with open(filename, 'rb') as f:
        obj = pickle.load(f)
    return obj
