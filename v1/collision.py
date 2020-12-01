import numpy as np

from params import p
from general_functions import find_particle_radius


def inelastic_collision(r1, r2, v1, v2, m1, m2):
    """
    input:  v1, v2 are velocities of two particles that are about to collide
            m1, m2 are the masses of the particles
            r1, r2 are the positions of the particles
    output: v is the velocity of the resultant particle
            m is the mass of the resultant particle
            r is the position of the resultant particle
    """

    m = m1 + m2
    r = (m1 * r1 + m2 * r2) / m
    v = (m1 * v1 + m2 * v2) / m

    return r, v, m


def get_particle_pairs(n):
    """returns all possible combinations of particle pairs"""
    return [(i, j) for i in range(0, n) for j in range(i+1, n)]


def coalesce(collision_pair, rs, vs, ms):
    """update rs, vs and ms after a collision"""
    r1 = rs[collision_pair[0]]
    r2 = rs[collision_pair[1]]
    v1 = vs[collision_pair[0]]
    v2 = vs[collision_pair[1]]
    m1 = ms[collision_pair[0]]
    m2 = ms[collision_pair[1]]
    r, v, m = inelastic_collision(r1, r2, v1, v2, m1, m2)

    assert collision_pair[1] > collision_pair[0]
    rs = np.delete(rs, collision_pair[1], 0)
    rs = np.delete(rs, collision_pair[0], 0)
    vs = np.delete(vs, collision_pair[1], 0)
    vs = np.delete(vs, collision_pair[0], 0)
    ms = np.delete(ms, collision_pair[1])
    ms = np.delete(ms, collision_pair[0])

    rs = np.vstack([rs, r])
    vs = np.vstack([vs, v])
    ms = np.hstack([ms, m])

    return rs, vs, ms


def combine_overlapping_particles(rs, vs, ms):
    """Useful for generating initial conditions, and making sure no overlaps occur after a coalesce"""

    def find_first_overlapping_pair(rs, radii):
        for i in range(len(rs)):
            for j in range(i + 1, len(rs)):
                dist = np.linalg.norm(rs[i] - rs[j])
                min_dist = radii[i] + radii[j]
                if dist < min_dist:
                    return i, j
        return None

    while True:
        overlapping_pair = find_first_overlapping_pair(rs, find_particle_radius(ms))
        if overlapping_pair is None:
            break
        print('there is a collision')
        rs, vs, ms = coalesce(overlapping_pair, rs, vs, ms)

    return rs, vs, ms


class CollisionEvent:
    """Watches for collisions between particles. solve_ivp is interrupted in case of collision"""
    def __init__(self, i, j):
        # i, j are the indexes of the particles to watch
        self.i = i
        self.j = j
        self.terminal = True

    def __call__(self, t, y, ms, N):
        rs = y[:int(len(y) / 2)].reshape((N, p.num_dimensions))

        disp = rs[self.i] - rs[self.j]
        dist = np.linalg.norm(disp)

        radius1 = find_particle_radius(ms[self.i])
        radius2 = find_particle_radius(ms[self.j])
        collision_radius = radius1 + radius2

        return dist - collision_radius

    def __repr__(self):
        return "<Collision event for particles {} and {}>".format(self.i, self.j)


def find_collision_pair_index(t_events):
    """Which particles collided?"""
    index = None
    for i, t_event in enumerate(t_events):
        if len(t_event) == 1:
            assert index is None  # else two collisions occured at the same time
            index = i

    return index