from dataclasses import dataclass
import numpy as np
import pickle
from typing import Union


@dataclass
class ICAAngularMomentum:
    """the same angular momentum is added to each particle"""
    angular_momentum: float


@dataclass
class ICAAngularVelocity:
    """the same angular velocity is added to each particle"""
    angular_velocity: float


@dataclass
class ICACircularOrbit:
    """All particles are given enough velocity to give them a circular orbit"""
    pass


@dataclass
class ICACircularOrbitCentralMass:
    """All particles are given enough velocity to give them a circular orbit around a single mass at the origin.
    Other masses are ignored"""
    central_mass: float


@dataclass
class ICSphericalExponential:
    """particles will be distributed radially, with probability decreasing exponentially with radius"""
    scale: float


@dataclass
class ICSphericalUniform:
    """Particles distributed uniformly within a sphere"""
    max_r: float


@dataclass
class ICDiscUniform:
    """Particles distributed uniformly withing a flat disc"""
    max_r: float
    height: float


@dataclass
class ConstDensity:
    """All particles have the same density. Particle radius increases with the cube root of mass"""
    radius_of_unit_mass_particle: float = 0.1


class ConstDensityThenCollapse:
    """All particles have the same density, up until a critical point where the radius collapses to a constant"""
    radius_of_unit_mass_particle: float = 0.1
    critical_mass: float = 1000
    collapsed_radius: float = 0.1


@dataclass(frozen=True)
class Params:
    # Simulation settings
    use_given_ics = False
    num_particles = 20   # including extra particles
    num_dimensions = 3
    max_v = 0  # add a random velocity vector of maximum magnitude max_v
    ic: Union[ICSphericalUniform, ICDiscUniform, ICSphericalExponential] = ICDiscUniform(70, 2)
    min_m = 1
    max_m = 2
    tot_t: int = 100
    G = 1  # 10
    gravity_on = True
    collisions_on = False
    radius_function: Union[ConstDensity, ConstDensityThenCollapse] = ConstDensityThenCollapse()

    # add extra particles with custom rs and ms. ICAs will not be applied to these particles.
    # p.num_particles will be updated in generate_initial_conditions
    extra_particle_rs = np.array([[0, 0, 0]])
    extra_particle_vs = np.array([0, 0, 0])
    extra_particle_ms = np.array([1000])
    num_particles_to_generate = int(num_particles - len(extra_particle_ms))

    # advanced settings
    snapshots = int(tot_t * 1)
    max_step = 0.1     # the max step size for solve_ivp. It is particularly important to set this when gravity is off
    qt = 1             # quotient threshold for octree approximation

    # how does the simulation calculate accelerations? "adaptive" uses py_brute below n=use_brute_below and cpp_octree above
    calc_types = {1: "py_brute", 2: "py_octree", 3: "cpp_octree", 4: "adaptive"}
    calc_type = 3
    use_brute_below = 200
    rtol = 1     # default = 1e-3
    atol = 0.001     # default = 1e-6

    # Initial condition adjustment types
    ica: Union[None,
               ICAAngularVelocity,
               ICAAngularMomentum,
               ICACircularOrbit,
               ICACircularOrbitCentralMass] = ICACircularOrbitCentralMass(1000)

    # Visualisation settings
    fps = 30
    display_rate = 1           # display every n snapshots
    dt = tot_t / snapshots
    display_size = 100
    camera_coords = np.array((0, 0, -20))
    z_coord_of_projection_plane = 0

    # Files
    ic_filename = 'ics.pkl'      # initial conditions
    params_filename = 'params.pkl'
    save_continuously = True

    def __post_init__(self):
        assert self.num_dimensions >= 2
        assert self.snapshots % self.display_rate == 0
        assert len(self.extra_particle_rs) == len(self.extra_particle_ms)

        if self.params_filename:
            with open(self.params_filename, 'wb') as f:
                pickle.dump(self, f)



p = Params()
