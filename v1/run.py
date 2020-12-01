from solar_system import run_sim, unpickle
from visualise import plot, animate_top_side
from params import p


sol_filename = 'solution.pkl'
ics_filename = 'ics.pkl'
# animations can be saved to .gif or .mp4. .mp4 has a better frame rate, but requires an ffmpeg writer.
animation_filename = "animation2.gif"

# ics = unpickle(ics_filename)
# solution = unpickle(sol_filename)

solution = run_sim(filename=sol_filename, ics=None)
animate_top_side(solution, animation_filename)
