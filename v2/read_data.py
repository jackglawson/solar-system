import pandas as pd


def read_data(filename = "simulation.dat"):
    return pd.read_table(filename, sep=" ", index_col=False)

