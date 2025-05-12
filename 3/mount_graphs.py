import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from scipy.stats import pearsonr

if __name__ == "__main__":
    file = "result_bias.csv"
    df = pd.read_csv(file)
    df["result"] = df["result"].astype(bool)

    prob = df.groupby(["win", "bias"])["result"].mean().reset_index()

    data = prob.pivot(index="bias", columns="win", values="result")

    # Create the heatmap
    sns.heatmap(data, annot=True, cmap="coolwarm", vmin=0, vmax=1)
    plt.xlabel("Taxa de jogos já jogados")
    plt.ylabel("Viés da equipe 1")

    plt.savefig("graphs/heatmap.eps")
    plt.clf()

    file = "result_time.csv"
    df = pd.read_csv(file)
    x = df["teams"]
    y = df["time"]
    plt.scatter(x, y)

    plt.xlabel("Quantidade de equipes")
    plt.ylabel("Tempo (ms)")

    coeffs = np.polyfit(x, y, 4)
    poly = np.poly1d(coeffs)
    x_fit = np.linspace(min(x), max(x), 50)
    y_fit = poly(x_fit)
    r, _ = pearsonr(x, y)
    plt.plot(x_fit, y_fit, color="red", label=f"Regressão grau 4, r={r:.3f}")

    plt.legend()
    plt.savefig("graphs/teams.eps")
    plt.clf()
