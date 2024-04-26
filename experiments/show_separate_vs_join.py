import pandas as pd
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="This script shows the graph of the separate vs joint launch.")

    parser.add_argument('arg', type=str, help="Path to the data")

    args = parser.parse_args()

    df = pd.read_csv(args.arg)
    df = df.groupby(by=['protocol', 'joint', 't', 'instances', 'security']).agg({'time': 'mean'})
    print(df)


