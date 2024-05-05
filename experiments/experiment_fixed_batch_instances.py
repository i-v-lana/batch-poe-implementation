import argparse
from run_helpers import all_but_naive_run
import pandas as pd
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="This script runs all the protocols for several times and writes the result to the output file.")

    parser.add_argument('--exp', type=int, help="Number of experiments")
    parser.add_argument('--inst', type=int, help="Number of batch instances")
    parser.add_argument('--build_path', type=str, help="Path to the built project")
    parser.add_argument('--output_file', type=str, help="Path to the output file")

    args = parser.parse_args()

    print("Argument exp:", args.exp)
    print("Argument inst:", args.inst)
    print("Argument build_path:", args.build_path)
    print("Argument output_file:", args.output_file)

    run_args = {'protocol_type': '',
                'logt': 25,
                'experiments': 1,
                'instances_per_exp': args.inst,
                'build_path': args.build_path,
                'output_file': 'tmp.csv'
                }
    df = pd.DataFrame()
    for i in range(args.exp):
        all_but_naive_run(run_args)
        cur_df = pd.read_csv(run_args['output_file'])
        df = pd.concat([df, cur_df], ignore_index=True)
        os.remove(run_args['output_file'])

    df.to_csv(args.output_file)


