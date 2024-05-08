import math

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


if __name__ == "__main__":
    possible_cnt = [10**i for i in range(2, 15)]
    for cur_cnt in possible_cnt:
        print("exponent", mult_exponent(cur_cnt))
        print("hybrid", mult_hybrid(cur_cnt))
        print("bucket", mult_bucket(cur_cnt))



