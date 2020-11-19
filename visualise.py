import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter, FFMpegWriter


def plot(df):
    for i, row in df.iterrows():
        plt.scatter(row.rx, row.ry, c="k")
    plt.show()


def animate_top_side(df,
                     display_size: float = None,
                     display_rate: int = 1,
                     fps: int = 30,
                     filename="animation.gif", ):

    """takes the objects and animates them using funcanimation"""

    # defined quantities
    timestamps = pd.unique(df.timestamp)
    num_of_frames = int(len(timestamps) / display_rate)
    frame_interval = 1000 / fps
    num_particles = len(df[df.timestamp == 0])

    # set up fig
    fig, (ax_top, ax_side) = plt.subplots(1, 2)
    fig.set_figwidth(12)
    ax_top.axis('scaled')
    ax_side.axis('scaled')
    ax_top.set_title('Top view')
    ax_side.set_title('Side view')
    ax_top.set_xlabel('x'), ax_top.set_ylabel('y')
    ax_side.set_xlabel('x'), ax_side.set_ylabel('z')

    min_x = min(df.rx)
    max_x = max(df.rx)
    min_y = min(df.ry)
    max_y = max(df.ry)
    min_z = min(df.rz)
    max_z = max(df.rz)

    if display_size:
        ax_top.set_xlim(-display_size, display_size)
        ax_top.set_ylim(-display_size, display_size)
        ax_side.set_xlim(-display_size, display_size)
        ax_side.set_ylim(-display_size, display_size)
    else:
        ax_top.set_xlim(min_x, max_x)
        ax_top.set_ylim(min_y, max_y)
        ax_side.set_xlim(min_x, max_x)
        ax_side.set_ylim(min_z, max_z)

    circles_top = [plt.Circle((0, 0), 1) for _ in range(num_particles)]
    circles_side = [plt.Circle((0, 0), 1) for _ in range(num_particles)]
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
        t = timestamps[i * display_rate]
        N = len(df[df.timestamp == t])

        xs = tuple(df[df.timestamp == t].rx)
        ys = tuple(df[df.timestamp == t].ry)
        zs = tuple(df[df.timestamp == t].rz)
        radii = tuple(df[df.timestamp == t].radius)
        for j in range(N):
            circles_top[j].set_center((xs[j], ys[j]))
            circles_top[j].set_radius(radii[j])
            circles_side[j].set_center((xs[j], zs[j]))
            circles_side[j].set_radius(radii[j])

        # remove particles that no longer exist
        for j in range(N, num_particles):
            circles_top[j].set_center((max_x + 1000, max_y + 1000))
            circles_side[j].set_center((max_x + 1000, max_z + 1000))

        return circles_top, circles_side

    ani = FuncAnimation(
        fig, update_screen, num_of_frames,
        init_func=init, blit=False, interval=frame_interval)

    if filename:
        print("saving animation to {}".format(filename))
        if filename[-4:] == ".gif":
            writer = PillowWriter(fps=fps)
        elif filename[-4:] == ".mp4":
            writer = FFMpegWriter(fps=fps)
        else:
            raise Exception("Animation file format not allowed")
        ani.save(filename, writer=writer)

    plt.show()
