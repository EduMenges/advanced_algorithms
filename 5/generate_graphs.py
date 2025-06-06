import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import linregress

if __name__ == "__main__":
    df_n = pd.read_csv("results_n.csv")

    def generate_n(column):
        x = df_n["n"]
        y = df_n[column]

        slope, intercept, r_value, p_value, std_err = linregress(x, y)
        plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

        plt.scatter(x, y, label="Observações individuais")

        plt.xlabel("Número de elementos")
        plt.ylabel("Tempo (ms)")
        plt.legend()

        plt.savefig(f"graphs/n_{column}.png")
        plt.clf()

    generate_n("time_dp")
    generate_n("time_hexu")

    df_cap = pd.read_csv("results_capacity.csv")
    df_cap["cap_norm"] = df_cap["capacity"] / max(df_cap["capacity"])

    def generate_capacity(column):
        x = df_cap["cap_norm"]
        y = df_cap[column]

        slope, intercept, r_value, p_value, std_err = linregress(x, y)
        plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

        plt.scatter(x, y, label="Observações individuais")

        plt.xlabel("Capacidade")
        plt.ylabel("Tempo (ms)")
        plt.legend()

        plt.savefig(f"graphs/capacity_{column}.png")
        plt.clf()

    generate_capacity("time_dp")
    generate_capacity("time_hexu")

    df_wmax = pd.read_csv("results_wmax.csv")

    def generate_wmax(column):
        x = df_wmax["max_weight"]
        y = df_wmax[column]

        slope, intercept, r_value, p_value, std_err = linregress(x, y)
        plt.plot(x, intercept + slope * x, 'r', label=f"Regressão linear (r={round(r_value,2)})")

        plt.scatter(x, y, label="Observações individuais")

        plt.xlabel("Peso máximo")
        plt.ylabel("Tempo (ms)")
        plt.legend()

        plt.savefig(f"graphs/wmax_{column}.png")
        plt.clf()

    generate_wmax("time_dp")
    generate_wmax("time_hexu")
