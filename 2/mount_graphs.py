import pandas as pd
import matplotlib.pyplot as plt

if __name__ == "__main__":
    df = pd.read_csv("edge_stats.csv")

    x = df["edges"]

    plt.scatter(x, df["dfs time (ms)"], label="DFS")
    plt.scatter(x, df["bfs time (ms)"], label="BFS")
    plt.scatter(x, df["fat time (ms)"], label="Fattest path")

    plt.xlabel("Edge amount")
    plt.ylabel("Time (ms)")
    plt.legend()

    plt.savefig("edge_x_time_edges.png", format="png")
    plt.clf()

    plt.scatter(x, df["dfs vertices"], label="DFS")
    plt.scatter(x, df["bfs vertices"], label="BFS")
    plt.scatter(x, df["fat vertices"], label="Fattest path")

    plt.xlabel("Edge amount")
    plt.ylabel("Vertices touched")
    plt.legend()

    plt.savefig("vertices_touched_edges.png", format="png")
    plt.clf()

    plt.scatter(x, df["dfs edges"], label="DFS")
    plt.scatter(x, df["bfs edges"], label="BFS")
    plt.scatter(x, df["fat edges"], label="Fattest path")

    plt.xlabel("Edge amount")
    plt.ylabel("Edges touched")
    plt.legend()

    plt.savefig("edges_touched_edges.png", format="png")
    plt.clf()

    df = pd.read_csv("max_flow_stats.csv")

    x = df["max_flow"]

    plt.scatter(x, df["dfs time (ms)"], label="DFS")
    plt.scatter(x, df["bfs time (ms)"], label="BFS")
    plt.scatter(x, df["fat time (ms)"], label="Fattest path")

    plt.xlabel("Edge amount")
    plt.ylabel("Time (ms)")
    plt.legend()

    plt.savefig("edge_x_time_max_flow.png", format="png")
    plt.clf()

    plt.scatter(x, df["dfs vertices"], label="DFS")
    plt.scatter(x, df["bfs vertices"], label="BFS")
    plt.scatter(x, df["fat vertices"], label="Fattest path")

    plt.xlabel("Edge amount")
    plt.ylabel("Vertices touched")
    plt.legend()

    plt.savefig("vertices_touched_max_flow.png", format="png")
    plt.clf()

    plt.scatter(x, df["dfs edges"], label="DFS")
    plt.scatter(x, df["bfs edges"], label="BFS")
    plt.scatter(x, df["fat edges"], label="Fattest path")

    plt.xlabel("Edge amount")
    plt.ylabel("Edges touched")
    plt.legend()

    plt.savefig("edges_touched_max_flow.png", format="png")
    plt.clf()

