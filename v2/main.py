from read_data import read_data
from visualise import animate_top_side


df = read_data()

display_size = 10
filename = "animation.gif"
animate_top_side(df, display_size=display_size, filename=filename, fps=60, display_rate=2)
