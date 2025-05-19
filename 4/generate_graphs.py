import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

def model(x, a, b):
    return a * (x ** 2) + b * (x ** 2) * np.log(x)


if __name__ == "__main__":
    df = pd.read_csv("results.csv")

    x = df["n"]
    y = df["time"]
    plt.scatter(x, y)

    params, covariance = curve_fit(model, x, y)

    a_fit = params[0]
    b_fit = params[1]
    y_fit = model(x.unique(), a_fit, b_fit)

    plt.plot(x.unique(), y_fit, label="Regressão não linear de quadrados mínimos", color="red")

    plt.xlabel("Tamanho da matriz")
    plt.ylabel("Tempo (ms)")
    plt.savefig("graphs/n_time.eps")
