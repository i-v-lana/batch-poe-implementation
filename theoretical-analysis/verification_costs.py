import math
import matplotlib.pyplot as plt
import numpy as np

secpar = 128


def mult_exponent(cnt):
    global secpar
    return (3 * secpar + 2) * cnt


def mult_hybrid(cnt):
    global secpar
    return secpar * (cnt + 3 * secpar + 2)


def mult_bucket(_cnt):
    global secpar
    min_value = float("inf")
    optimal_k = 3

    def evaluate(cnt, k):
        return math.ceil(secpar * 1.0 / (k - 2)) * (2 * cnt + (3 * k + 2) * (2 ** k) + (3 * secpar + 2))

    for _k in range(3, secpar + 1):
        current_value = evaluate(_cnt, _k)
        if current_value < min_value:
            min_value = current_value
            optimal_k = _k

    return evaluate(_cnt, optimal_k)


def mult_graph():
    fig, ax = plt.subplots()

    ax.scatter(possible_cnt, exponents_result, marker='o', label='Random Exponents', color='blue')
    ax.scatter(possible_cnt, hybrid_result, marker='s', label='Hybrid', color='black')
    ax.scatter(possible_cnt, bucket_result, marker='d', label='Bucket', color='red')

    ax.plot(possible_cnt, exponents_result, linestyle='-', color='blue', alpha=0.5)
    ax.plot(possible_cnt, hybrid_result, linestyle='-', color='black', alpha=0.5)
    ax.plot(possible_cnt, bucket_result, linestyle='-', color='red', alpha=0.5)

    ax.set_xlabel('number of instances m')
    ax.set_ylabel('verification cost')

    ax.legend()
    ax.grid(True, linestyle='--')
    ax.set_xscale('log')
    ax.set_yscale('log')
    plt.savefig('absolute_mult.pdf', dpi=300, bbox_inches='tight')

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
    ax.set_ylabel('relative verification cost')

    ax.legend()
    ax.grid(True, linestyle='--')
    ax.set_xscale('log')
    plt.savefig('relative_mult.pdf', dpi=300, bbox_inches='tight')

    plt.show()


if __name__ == "__main__":
    possible_cnt = [10**i for i in range(2, 15)]

    exponents_result = [mult_exponent(cur_cnt) for cur_cnt in possible_cnt]
    hybrid_result = [mult_hybrid(cur_cnt) for cur_cnt in possible_cnt]
    bucket_result = [mult_bucket(cur_cnt) for cur_cnt in possible_cnt]

    rel_exponents_result = [exponents_result[i] / exponents_result[i] for i in range(len(exponents_result))]
    rel_hybrid_result = [hybrid_result[i] / exponents_result[i] for i in range(len(hybrid_result))]
    rel_bucket_result = [bucket_result[i] / exponents_result[i] for i in range(len(bucket_result))]

    mult_graph()
    rel_mult_graph()