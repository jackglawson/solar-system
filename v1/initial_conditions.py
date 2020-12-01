import pickle
import numpy as np
from scipy.optimize import root

from params import p, ICSphericalExponential, ICSphericalUniform, ICDiscUniform, ICAAngularMomentum, ICAAngularVelocity, \
    ICACircularOrbit, ICACircularOrbitCentralMass
from collision import combine_overlapping_particles


def generate_spherical_exponential_rs():
    """
    The probability of finding a particle in dV at distance r is proportional to e^-r
    The probability of finding a particle in a shell of width dr at radius r is 1/2 * e^-r * r^2
    """
    assert p.num_dimensions == 3
    assert isinstance(p.ic, ICSphericalExponential)

    def cum_distr_radius(radius):
        scaled_r = radius / p.ic.scale
        return 1 - np.exp(-scaled_r) * (0.5 * scaled_r ** 2 + scaled_r + 1)
        # return 1 - np.exp(-radius) * (0.5 * radius ** 2 + radius + 1)

    def find_root(random):
        return root(lambda r: random - cum_distr_radius(r), 1).x

    find_roots = np.vectorize(find_root)

    randoms = np.random.uniform(0, 1, p.num_particles_to_generate)
    radii = find_roots(randoms)

    thetas = np.arccos(1 - 2 * np.random.uniform(0, 1, p.num_particles_to_generate))
    phis = np.random.uniform(0, 2 * np.pi, p.num_particles_to_generate)

    xs = radii * np.sin(thetas) * np.cos(phis)
    ys = radii * np.sin(thetas) * np.sin(phis)
    zs = radii * np.cos(thetas)

    rs = np.stack([xs, ys, zs], axis=1)

    return rs


def generate_spherical_uniform_rs():
    assert p.num_dimensions == 3
    assert isinstance(p.ic, ICSphericalUniform)

    randoms = np.random.uniform(0, 1, p.num_particles_to_generate)
    radii = p.ic.max_r * np.cbrt(randoms)
    thetas = np.arccos(1 - 2 * np.random.uniform(0, 1, p.num_particles_to_generate))
    phis = np.random.uniform(0, 2 * np.pi, p.num_particles_to_generate)

    xs = radii * np.sin(thetas) * np.cos(phis)
    ys = radii * np.sin(thetas) * np.sin(phis)
    zs = radii * np.cos(thetas)

    rs = np.stack([xs, ys, zs], axis=1)

    return rs


def generate_disc_uniform_rs():
    assert p.num_dimensions == 3
    assert isinstance(p.ic, ICDiscUniform)

    randoms = np.random.uniform(0, 1, p.num_particles_to_generate)
    radii = p.ic.max_r * np.sqrt(randoms)
    phis = np.random.uniform(0, 2 * np.pi, p.num_particles_to_generate)

    xs = radii * np.cos(phis)
    ys = radii * np.sin(phis)
    zs = np.random.uniform(-p.ic.height/2, p.ic.height/2, p.num_particles_to_generate)

    rs = np.stack([xs, ys, zs], axis=1)

    return rs


def generate_initial_conditions():
    vs = np.random.uniform(-p.max_v, p.max_v, (p.num_particles_to_generate, p.num_dimensions))
    ms = np.random.uniform(p.min_m, p.max_m, p.num_particles_to_generate)

    if isinstance(p.ic, ICSphericalExponential):
        rs = generate_spherical_exponential_rs()
    elif isinstance(p.ic, ICSphericalUniform):
        rs = generate_spherical_uniform_rs()
    elif isinstance(p.ic, ICDiscUniform):
        rs = generate_disc_uniform_rs()
    else:
        raise Exception('Invalid IC type')

    thetas = np.arctan2(rs[:, 0], rs[:, 1])
    rhos = np.linalg.norm(rs[:, 0:2], axis=1)

    if p.ica is None:
        dvs = np.zeros((p.num_dimensions - 1, p.num_particles_to_generate))
    elif isinstance(p.ica, ICAAngularMomentum):
        dvs = p.ica.angular_momentum / (ms * rhos) * np.vstack([-np.cos(thetas), np.sin(thetas)])
    elif isinstance(p.ica, ICAAngularVelocity):
        dvs = p.ica.angular_velocity * rhos * np.vstack([-np.cos(thetas), np.sin(thetas)])
    elif isinstance(p.ica, ICACircularOrbit):
        if isinstance(p.ic, ICSphericalUniform):
            dvs = np.sqrt(p.G * np.sum(ms) * rhos**2 / p.ic.max_r**3) * np.vstack([-np.cos(thetas), np.sin(thetas)])
        else:
            raise Exception("ICACircularOrbit cannot be used with this IC")
    elif isinstance(p.ica, ICACircularOrbitCentralMass):
        dvs = np.sqrt(p.G * p.ica.central_mass / rhos) * np.vstack([-np.cos(thetas), np.sin(thetas)])
    else:
        raise Exception('Invalid ICA')

    dvs = dvs.T
    dvs = np.hstack([dvs, np.zeros((p.num_particles_to_generate, p.num_dimensions - 2))])
    vs = vs + dvs

    rs = np.vstack((rs, p.extra_particle_rs))
    vs = np.vstack((vs, p.extra_particle_vs))
    ms = np.hstack((ms, p.extra_particle_ms))

    rs, vs, ms = combine_overlapping_particles(rs, vs, ms)

    y_0 = np.concatenate([rs, vs]).flatten()

    print('Initialised {} particles'.format(p.num_particles))

    if p.ic_filename:
        with open(p.ic_filename, 'wb') as f:
            pickle.dump([y_0, ms], f)

    return y_0, ms


def give_ics():
    """
    Use for debugging purposes.
    Allows the initial conditions to be specified directly, rather than generated randomly
    """
    rs = np.array([[-0.5, 0, 0],
                   [5, 0, 0]])

    vs = np.array([[0, -0.2, 0],
                   [0, 2, 0]])

    ms = np.array([10, 1])

    assert len(rs) == len(vs) == len(ms) == p.num_particles

    rs, vs, ms = combine_overlapping_particles(rs, vs, ms)

    y_0 = np.concatenate([rs, vs]).flatten()

    print('INITIAL CONDITIONS:\ny_0 = {}\nms = {}'.format(y_0, ms))

    if p.ic_filename:
        with open(p.ic_filename, 'wb') as f:
            pickle.dump([y_0, ms], f)

    return y_0, ms
