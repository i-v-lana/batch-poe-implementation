import math
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd


def mult_graph():
    fig, ax = plt.subplots()

    ax.scatter(possible_cnt, exponents_result, marker='o', label='Random Exponents', color='blue')
    ax.scatter(possible_cnt, hybrid_result, marker='s', label='Hybrid', color='black')
    ax.scatter(possible_cnt, bucket_result, marker='d', label='Bucket', color='red')

    ax.plot(possible_cnt, exponents_result, linestyle='-', color='blue', alpha=0.5)
    ax.plot(possible_cnt, hybrid_result, linestyle='-', color='black', alpha=0.5)
    ax.plot(possible_cnt, bucket_result, linestyle='-', color='red', alpha=0.5)

    ax.set_xlabel('number of instances m')
    ax.set_ylabel('verification cost (experiment)')

    ax.legend()
    ax.grid(True, linestyle='--')
    ax.set_xscale('log')
    ax.set_yscale('log')
    plt.savefig('absolute_mult_exp.pdf', dpi=300, bbox_inches='tight')

    plt.show()

def rel_mult_graph():
    fig, ax = plt.subplots()

    ax.scatter(possible_cnt, rel_exponents_result, marker='o', label='Random Exponents', color='blue')
    ax.scatter(possible_cnt, rel_hybrid_result, marker='s', label='Hybrid', color='black')
    ax.scatter(possible_cnt, rel_bucket_result, marker='d', label='Bucket', color='red')

    ax.plot(possible_cnt, rel_exponents_result, linestyle='-', color='blue', alpha=0.5)
    ax.plot(possible_cnt, rel_hybrid_result, linestyle='-', color='black', alpha=0.5)
    ax.plot(possible_cnt, rel_bucket_result, linestyle='-', color='red', alpha=0.5)

    ax.set_xlabel('number of instances m')
    ax.set_ylabel('relative verification cost (experiment)')

    ax.legend()
    ax.grid(True, linestyle='--')
    ax.set_xscale('log')
    plt.savefig('relative_mult_exp.pdf', dpi=300, bbox_inches='tight')

    plt.show()


if __name__ == "__main__":
    results_path = "results"

    results_df = pd.DataFrame()
    for file in os.listdir(results_path):
        if file.endswith(".csv"):
            results_df = pd.concat([results_df, pd.read_csv(os.path.join(results_path, file))], ignore_index=True)
    results_df = results_df[["protocol", "instances", "time"]].sort_values(by=['protocol', 'instances']).copy()
    results_df = results_df.groupby(by=['protocol', 'instances']).agg({'time': 'mean'}).reset_index()
    print(results_df)

    exponents_result = results_df[results_df['protocol'] == 'exponent']['time'].tolist()
    hybrid_result = results_df[results_df['protocol'] == 'hybrid']['time'].tolist()
    bucket_result = results_df[results_df['protocol'] == 'bucket']['time'].tolist()
    possible_cnt = results_df['instances'].unique().tolist()

    rel_exponents_result = [exponents_result[i] / exponents_result[i] for i in range(len(exponents_result))]
    rel_hybrid_result = [hybrid_result[i] / exponents_result[i] for i in range(len(hybrid_result))]
    rel_bucket_result = [bucket_result[i] / exponents_result[i] for i in range(len(bucket_result))]

    mult_graph()
    rel_mult_graph()