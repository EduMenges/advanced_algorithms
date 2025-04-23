import pandas as pd
from scipy.stats import linregress
import matplotlib.pyplot as plt

out_directory = "graphs"

def draw_edges(df, col, label):
    x = df["edges"]
    y = df[f"{col} time (ms)"]
    plt.scatter(x, y, label=label)

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

    plt.xlabel("Quantidade de arestas")
    plt.ylabel("Tempo (ms)")
    plt.legend()

    plt.savefig(f"{out_directory}/edge_x_time_{col}.eps")
    plt.clf()

    y = df[f"{col} vertices"]
    plt.scatter(x, y, label=label)

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

    plt.xlabel("Quantidade de arestas")
    plt.ylabel("Vértices tocados")
    plt.legend()

    plt.savefig(f"{out_directory}/vertices_touched_edges_{col}.eps")
    plt.clf()

    y = df[f"{col} edges"]
    plt.scatter(x, y, label=label)

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

    plt.xlabel("Quantidade de arestas")
    plt.ylabel("Arestas tocadas")
    plt.legend()

    plt.savefig(f"{out_directory}/edges_touched_edges_{col}.eps")
    plt.clf()

def draw_flow(df, col, label):
    x = df["max_flow"]
    y = df[f"{col} time (ms)"]
    plt.scatter(x, y, label=label)

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

    plt.xlabel("Max flow")
    plt.ylabel("Time (ms)")
    plt.legend()

    plt.savefig(f"{out_directory}/max_flow_x_time_{col}.eps")
    plt.clf()

    y = df[f"{col} vertices"]
    plt.scatter(x, y, label=label)

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

    plt.xlabel("Max flow")
    plt.ylabel("Vertices touched")
    plt.legend()

    plt.savefig(f"{out_directory}/vertices_touched_max_flow_{col}.eps")
    plt.clf()

    y = df[f"{col} edges"]
    plt.scatter(x, y, label=label)

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

    plt.xlabel("Max flow")
    plt.ylabel("Edges touched")
    plt.legend()

    plt.savefig(f"{out_directory}/edges_touched_max_flow_{col}.eps")
    plt.clf()

if __name__ == "__main__":
    df = pd.read_csv("edge_stats.csv")

    samples = {"dfs": "DFS", "bfs": "BFS", "fat": "Fattest Path"}


    for col, label in samples.items():
        draw_edges(df, col, label)

    df = pd.read_csv("max_flow_stats.csv")

    for col, label in samples.items():
        draw_flow(df, col, label)
