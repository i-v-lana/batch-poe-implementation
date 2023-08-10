//
// Created by Светлана Иванова on 19.04.2023.
//

#include "batching.h"
#include "mpz_helper.h"
#include <iostream>

const int MAX_CNT_BITS = 1000;

Batching::Batching(int _lambda, int _k, int _t, int _cnt, int _lambda_batch, bigint _N) {
    t = _t;
    cnt = _cnt;
    N = _N;
    k = _k;
    /// length of an input of PRF function
    int logcnt = 4;
    // int logcnt = std::bitset<MAX_CNT_BITS>(cnt).count();
    /// length of an output of PRF function
    int low_order = 3;
    helper = mpz_helper();
    std::cout << "We're working in the group modulo " << N.num << "." << std::endl;
    std::cout << "PRF takes " << logcnt << " bits as an input and generates " << low_order << " bits as an output." << std::endl;
    prf = PRF(helper.get_random(_lambda_batch), _lambda_batch, logcnt, low_order);
    gen();
}

void Batching::batch() {
    alpha.clear();
    alpha.resize(cnt);
    for (int i = 0; i < cnt; ++i) {
        // std::cout << "new i: " << i << std::endl;
        alpha[i] = prf.gen(i);
    }
    bigint batch_x = bigint(1);
    bigint batch_y = bigint(1);
    for (int i = 0; i < cnt; ++i) {
        bigint xi = helper.pow(x[i], alpha[i], N);
        bigint yi = helper.pow(y[i], alpha[i], N);
        batch_x = helper.mul_mod(batch_x, xi, N);
        batch_y = helper.mul_mod(batch_y, yi, N);
        std::cout << "cur x: " << x[i].num << "; cur alpha:" << alpha[i].num << "; cur x^alpha:" << xi.num << std::endl;
        std::cout << "cur y: " << y[i].num << "; cur alpha:" << alpha[i].num << "; cur y^alpha:" << yi.num << std::endl;
    }
    std::cout << "batched instance" << std::endl;
    std::cout << "x: " << batch_x.num << "; y:" << batch_y.num << std::endl;
    bool result = run_vdf(batch_x, batch_y);
}

void Batching::print(std::ofstream& file) {
    for (int i = 0; i < cnt; ++i) {
        file << alpha[i].num << std::endl;
    }
}

void Batching::gen() {
    x.clear();
    y.clear();
    x.resize(cnt);
    y.resize(cnt);
    for (int i = 0; i < cnt; ++i) {
        x[i] = helper.get_random_mod(N);
        y[i] = helper.get_random_mod(N);
    }
}

bool Batching::run_vdf(bigint cur_x, bigint cur_y) {
    bigint real_y = trapdoor(cur_x);
    Wesolowski vdf = Wesolowski();
    vdf.setup(lambda, k, N.num);
    bigint l = bigint();
    bigint pi = bigint();
    vdf.evaluate(l.num, pi.num, cur_x.num, t);
    bool not_failed = vdf.naive_verify(cur_x.num, t, l.num, pi.num);
    if (!not_failed) {
        std::cout << "ERROR\n" << std::endl;
        return false;
    }
    if (cur_y == vdf.y_saved)
        std::cout << cur_x << " to the power of 2^" << t << "equals " << cur_y << std::endl;
    else
        std::cout << "expected " << vdf.y_saved << std::endl;
    return true;
}

bigint Batching::trapdoor(bigint x) {
    if (helper.gcd(x, N) != 1) {
        std::cout << "can't use the trapdoor, going to use naive computation" << std::endl;

    }
    /// N = pq -> phi = (p - 1) * (q - 1)
    bigint phi = (p - 1UL) * (q - 1UL);
    /// counting 2^t mod phi
    bigint power = helper.pow(bigint(2), bigint(t), phi);
    bigint ans = helper.pow(x, power, N);
    return ans;
}

void Batching::set_trapdoor(bigint& _p, bigint& _q) {
    if (N == _p * _q) {
        p = _p;
        q = _q;
        std::cout << "trapdoor was set" << std::endl;
    } else {
        std::cout << "trapdoor wasn't set" << std::endl;
    }
}
