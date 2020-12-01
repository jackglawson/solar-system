import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter, FFMpegWriter
from params import p
from general_functions import find_particle_radius


def plot(whole_solution):
    for i in range(p.num_particles):
        plt.scatter(*whole_solution.rs[:, i, 0:2].T)
    plt.show()


def plot_snapshot(rs, ms):
    circles = [plt.Circle(rs[i, (0, 1)], find_particle_radius(ms[i])) for i in range(len(ms))]
    ax = plt.axes()
    ax.axis('scaled')
    ax.set_xlim(np.min(rs[:, 0])-1, np.max(rs[:, 0])+1)
    ax.set_ylim(np.min(rs[:, 1])-1, np.max(rs[:, 1])+1)
    for circle in circles:
        ax.add_artist(circle)
    plt.show()


def animate_top_side(whole_solution, filename='', ):
    """takes the objects and animates them using funcanimation"""

    # defined quantities
    num_of_frames = int(p.snapshots / p.display_rate)
    frame_interval = 1000 / p.fps

    # fig = plt.figure(figsize=(12, 12))
    # ax = plt.axes()
    fig, (ax_top, ax_side) = plt.subplots(1, 2)
    fig.set_figwidth(12)
    ax_top.axis('scaled')
    ax_side.axis('scaled')
    ax_top.set_title('Top view')
    ax_side.set_title('Side view')
    ax_top.set_xlabel('x'), ax_top.set_ylabel('y')
    ax_side.set_xlabel('x'), ax_side.set_ylabel('z')

    min_x = np.nanmin(whole_solution.rs[:, :, 0])
    max_x = np.nanmax(whole_solution.rs[:, :, 0])
    min_y = np.nanmin(whole_solution.rs[:, :, 1])
    max_y = np.nanmax(whole_solution.rs[:, :, 1])
    min_z = np.nanmin(whole_solution.rs[:, :, 2])
    max_z = np.nanmax(whole_solution.rs[:, :, 2])

    if p.display_size:
        ax_top.set_xlim(-p.display_size, p.display_size)
        ax_top.set_ylim(-p.display_size, p.display_size)
        ax_side.set_xlim(-p.display_size, p.display_size)
        ax_side.set_ylim(-p.display_size, p.display_size)
    else:
        ax_top.set_xlim(min_x, max_x)
        ax_top.set_ylim(min_y, max_y)
        ax_side.set_xlim(min_x, max_x)
        ax_side.set_ylim(min_z, max_z)

    circles_top = [plt.Circle((0, 0), find_particle_radius(1)) for _ in range(p.num_particles)]
    circles_side = [plt.Circle((0, 0), find_particle_radius(1)) for _ in range(p.num_particles)]
    for circle in circles_top:
        ax_top.add_artist(circle)
    for circle in circles_side:
        ax_side.add_artist(circle)

    def init():
        """initialisation function for FuncAnimation"""
        for i in range(len(circles_top)):
            circles_top[i].set_center((0, 0))
            circles_top[i].set_color('b')
            circles_top[i].set_radius(1)
        for i in range(len(circles_side)):
            circles_side[i].set_center((0, 0))
            circles_side[i].set_color('b')
            circles_side[i].set_radius(1)

        return circles_top, circles_side

    def update_screen(i):
        """update function for FuncAnimation"""
        i = i * p.display_rate
        N = int(whole_solution.Ns[i])
        for j in range(N):
            circles_top[j].set_center(whole_solution.rs[i, j, (0, 1)])
            circles_top[j].set_radius(find_particle_radius(whole_solution.ms[i, j]))
            circles_side[j].set_center(whole_solution.rs[i, j, (0, 2)])
            circles_side[j].set_radius(find_particle_radius(whole_solution.ms[i, j]))

        # remove particles that no longer exist
        for j in range(N, p.num_particles):
            circles_top[j].set_center((max_x + 1000, max_y + 1000))
            circles_side[j].set_center((max_x + 1000, max_z + 1000))

        return circles_top, circles_side

    ani = FuncAnimation(
        fig, update_screen, num_of_frames,
        init_func=init, blit=False, interval=frame_interval)

    if filename:
        print("saving animation to {}".format(filename))
        if filename[-4:] == ".gif":
            writer = PillowWriter(fps=p.fps)
        elif filename[-4:] == ".mp4":
            writer = FFMpegWriter(fps=p.fps)
        else:
            raise Exception("Animation file format not allowed")
        ani.save(filename, writer=writer)

    plt.show()

