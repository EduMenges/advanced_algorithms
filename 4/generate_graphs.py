import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit


def model(x, a, b):
    return a * (x ** 3) + b * (x ** 2) * np.log(x)


if __name__ == "__main__":
    df = pd.read_csv("results.csv")

    grouped = df.groupby("n")["time"]

    # Média (e desvio‑padrão) do tempo para cada tamanho
    x_mean = grouped.mean().index.values.astype(float)
    y_mean = grouped.mean().values
    y_std = grouped.std().values

    params, covariance = curve_fit(model, x_mean, y_mean)
    a_fit, b_fit = params

    y_fit = model(x_mean, a_fit, b_fit)

    ss_res = np.sum((y_mean - y_fit) ** 2)
    ss_tot = np.sum((y_mean - np.mean(y_mean)) ** 2)
    r_squared = 1 - ss_res / ss_tot

    plt.scatter(df["n"], df["time"], label="Observações individuais")

    plt.errorbar(x_mean, y_mean, yerr=y_std, fmt="o", color="black",
                 capsize=4, label="Média ± desvio‑padrão (ms)")

    x_fit = np.linspace(x_mean.min(), x_mean.max(), 300)
    y_fit = model(x_fit, a_fit, b_fit)
    plt.plot(x_fit, y_fit, color="red", label=f"Ajuste (R²={r_squared:.4f})")

    plt.xlabel("Tamanho da matriz")
    plt.ylabel("Tempo (ms)")
    plt.legend()

    plt.savefig("graphs/n_time.eps")
    plt.clf()

    x = df["n"]
    y = df["loop_iters"]
    plt.scatter(x, y, label="Observações individuais")

    coeffs = np.polyfit(x, y, 2)
    poly = np.poly1d(coeffs)
    x_fit = np.linspace(min(x), max(x), 50)
    y_fit = poly(x_fit)
    plt.plot(x_fit, y_fit, color="red")

    plt.xlabel("Tamanho da matriz")
    plt.ylabel("Iterações no loop principal")
    plt.legend()

    plt.savefig("graphs/main_loop.eps")
    plt.clf()

    x = df["n"]
    y = df["relaxations"]
    plt.scatter(x, y, label="Observações individuais")

    coeffs = np.polyfit(x, y, 2)
    poly = np.poly1d(coeffs)
    x_fit = np.linspace(min(x), max(x), 50)
    y_fit = poly(x_fit)
    plt.plot(x_fit, y_fit, color="red", label="Regressão polinomial de 2º grau")

    plt.xlabel("Tamanho da matriz")
    plt.ylabel("Relaxações")
    plt.legend()

    plt.savefig("graphs/relaxations.eps")
    plt.clf()

    x = df["n"]
    y = df["heap_ops"]
    plt.scatter(x, y, label="Observações individuais")

    coeffs = np.polyfit(x, y, 2)
    poly = np.poly1d(coeffs)
    x_fit = np.linspace(min(x), max(x), 50)
    y_fit = poly(x_fit)
    plt.plot(x_fit, y_fit, color="red", label="Regressão polinomial de 2º grau")

    plt.xlabel("Tamanho da matriz")
    plt.ylabel("Operações do heap")
    plt.legend()

    plt.savefig("graphs/heap_ops.eps")
    plt.clf()
