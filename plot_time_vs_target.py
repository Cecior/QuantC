import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

CONTEXT = "experiments/experiment_001"
CSV_PATH = CONTEXT + "/results.csv"
OUTPUT_IMAGE = CONTEXT + "/benchmark.png"

def main(): 
    print(f"Reading data from {CSV_PATH}...")

    try:
        df = pd.read_csv(CSV_PATH, skipinitialspace=True)
    except FileNotFoundError:
        print("Error: file not found. Check the .csv path")
        return

    sns.set_theme(style="whitegrid")

    plt.figure(figsize=(12, 6))
    sns.lineplot(
        data = df,
        x = "target",
        y = "time_ms",
        hue = "name_impl",
        marker = "o",
        linewidth = 2,
        markersize = 6
    )

    plt.title("Experiment 1 Benchmark", fontsize=16, fontweight="bold", pad=15)
    plt.xlabel("Target Qubit", fontsize=12, fontweight="bold")
    plt.ylabel("Execution Time [ms]", fontsize=12, fontweight="bold")

    max_qubit = df["target"].max()
    plt.xticks(range(0, int(max_qubit) + 1, 2))

    plt.legend(title="Implementation", title_fontsize="12", fontsize="10", loc="best")
    plt.tight_layout()


    plt.savefig(OUTPUT_IMAGE, dpi=300)
    print(f"Chart saved succesfully in {OUTPUT_IMAGE}")

    plt.show()

if __name__ == "__main__":
    main()