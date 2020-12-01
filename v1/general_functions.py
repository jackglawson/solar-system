import numpy as np
from params import p, ConstDensity, ConstDensityThenCollapse


def find_particle_radius(ms):
    """vectorised"""
    if isinstance(p.radius_function, ConstDensity):
        return np.cbrt(ms) * p.radius_function.radius_of_unit_mass_particle
    elif isinstance(p.radius_function, ConstDensityThenCollapse):
        def func(m):
            if m >= p.radius_function.critical_mass:
                return p.radius_function.collapsed_radius
            else:
                return np.cbrt(m) * p.radius_function.radius_of_unit_mass_particle
        return np.vectorize(func)(ms)
    else:
        raise Exception("Invalid particle radius function")


def find_acc(r_test, r, m):
    """find acceleration on a test mass, due to force from a particle at r with mass m"""
    disp = r_test - r
    dist = np.linalg.norm(disp)
    if dist < find_particle_radius(m):
        return disp * 0
    else:
        return - disp * p.G * m / dist ** 3


def find_total_acc(r_test: np.array, rs: np.array, ms: np.array):
    """find to total acceleration on a test mass due to particles at r. Ensure t_test is not in rs. Vectorised"""
    if np.sum(ms) == 0:
        return np.array([0,0,0])
    disps = r_test - rs
    dists = np.linalg.norm(disps, axis=1, keepdims=True)
    return np.sum(- disps * p.G * ms.reshape(len(ms), 1) / dists ** 3, axis=0)


def find_acc_v(rs: np.ndarray, ms: np.ndarray, n: int):
    r_mesh_1 = np.ndarray((n, n, p.num_dimensions))
    r_mesh_1[:, :, :] = rs
    r_mesh_2 = r_mesh_1.swapaxes(0, 1)

    disp_mesh = r_mesh_1 - r_mesh_2
    dist_mesh = np.linalg.norm(disp_mesh, axis=2)
    dist_mesh[dist_mesh == 0] = np.nan  # to prevent divide by zero error later

    m_mesh = np.ndarray((n, n))
    m_mesh[:, :] = ms

    particle_radius_mesh = find_particle_radius(m_mesh)
    # This will throw a warning because there are nans in the matrix.
    a_mesh = np.where(dist_mesh.reshape(n, n, 1) < particle_radius_mesh.reshape(n, n, 1),
                      0,
                      disp_mesh * p.G * m_mesh.reshape(n, n, 1) / dist_mesh.reshape(n, n, 1) ** 3)

    As = np.sum(np.nan_to_num(a_mesh, 0), axis=1)

    As += rs * p.H0**2

    return As


def particles_fall_in_cube(centre, width, rs):
    """Returns a boolean mask indicating which particles fall within the cube"""
    falls_within_bounds = np.all([centre - width/2 <= rs, rs < centre + width/2], axis=0)
    falls_within_all_bounds = np.all(falls_within_bounds, axis=1)
    return falls_within_all_bounds