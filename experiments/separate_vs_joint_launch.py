import argparse
from run_helpers import all_but_naive_run, subset_run, exponent_run, bucket_run, hybrid_run
import pandas as pd
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="This script compares the separate vs joint launch of the protocols.")

    parser.add_argument('--comp', type=int, help="Number of comparisons")
    parser.add_argument('--inst', type=int, help="Number of batch instances")
    parser.add_argument('--build_path', type=str, help="Path to the built project")
    parser.add_argument('--output_file', type=str, help="Path to the output file")

    args = parser.parse_args()

    print("Argument comp:", args.comp)
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
    times = {'joint': pd.DataFrame(), 'subset':  pd.DataFrame(), 'exponent':  pd.DataFrame(), 'bucket':  pd.DataFrame(), 'hybrid':  pd.DataFrame()}
    for i in range(args.comp):
        all_but_naive_run(run_args)
        df = pd.read_csv(run_args['output_file'])
        times['joint'] = pd.concat([times['joint'], df], ignore_index=True)
        os.remove(run_args['output_file'])

        subset_run(run_args)
        df = pd.read_csv(run_args['output_file'])
        times['subset'] = pd.concat([times['subset'], df], ignore_index=True)
        os.remove(run_args['output_file'])

        exponent_run(run_args)
        df = pd.read_csv(run_args['output_file'])
        times['exponent'] = pd.concat([times['exponent'], df], ignore_index=True)
        os.remove(run_args['output_file'])

        bucket_run(run_args)
        df = pd.read_csv(run_args['output_file'])
        times['bucket'] = pd.concat([times['bucket'], df], ignore_index=True)
        os.remove(run_args['output_file'])

        hybrid_run(run_args)
        df = pd.read_csv(run_args['output_file'])
        times['hybrid'] = pd.concat([times['hybrid'], df], ignore_index=True)
        os.remove(run_args['output_file'])

    final_df = times['joint']
    final_df['joint'] = True
    for key in times.keys():
        if key == 'joint':
            continue
        df = times[key]
        df['joint'] = False
        final_df = pd.concat([final_df, df], ignore_index=True)

    final_df.to_csv(args.output_file)


