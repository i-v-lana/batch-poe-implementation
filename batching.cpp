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
    /// alpha_i
    auto start_batching = std::chrono::high_resolution_clock::now();
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
    auto end_batching = std::chrono::high_resolution_clock::now();
    bigint _l = bigint();
    bigint _pi = bigint();
    batch_prover_part(&_l, &_pi, batch_x);
    std::pair<bool, long long> result = batch_verifier_part(batch_x, batch_y, _l, _pi);
    std::cout << "Total time of the Lior Rothem's protocol: " << (end_batching - start_batching).count() + result.second << std::endl;
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
        y[i] = trapdoor(x[i]);
    }
}

void Batching::naive_prover_part() {
    l.clear();
    l.resize(cnt);
    pi.clear();
    pi.resize(cnt);
    for (int i = 0; i < cnt; ++i) {
        Wesolowski vdf = Wesolowski();
        vdf.setup(lambda, k, N.num);
        vdf.prover(l[i].num, pi[i].num, x[i].num, t);
    }
}

bigint Batching::trapdoor(bigint& _x) {
    /// N = pq -> phi = (p - 1) * (q - 1)
    bigint phi = (p - 1UL) * (q - 1UL);
    /// counting 2^t mod phi
    bigint power = helper.pow(bigint(2), bigint(t), phi);
    bigint ans = helper.pow(_x, power, N);
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

void Batching::naive_verifier_part() {
    std::vector<bool> results(cnt, false);
    auto naive_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < cnt; ++i) {
        Wesolowski vdf = Wesolowski();
        vdf.setup(lambda, k, N.num);
        results[i] = vdf.verifier(x[i].num, y[i].num, t, l[i].num, pi[i].num);
    }
    auto naive_finish = std::chrono::high_resolution_clock::now();
    std::cout << "Total time of the naive approach " << (naive_finish - naive_start).count();
}

void Batching::naive_approach() {
    naive_prover_part();
    naive_verifier_part();
}

void Batching::batch_prover_part(bigint* _l, bigint* _pi, bigint batch_x) {
    Wesolowski vdf = Wesolowski();
    vdf.setup(lambda, k, N.num);
    vdf.prover(_l->num, _pi->num, batch_x.num, t);
}

std::pair<bool, long long> Batching::batch_verifier_part(bigint batch_x, bigint batch_y, bigint _l, bigint _pi) {
    auto wes_start = std::chrono::high_resolution_clock::now();
    Wesolowski vdf = Wesolowski();
    vdf.setup(lambda, k, N.num);
    bool result = vdf.verifier(batch_x.num, batch_y.num, t, _l.num, _pi.num);
    auto wes_end = std::chrono::high_resolution_clock::now();
    return std::make_pair(result, (wes_end - wes_start).count());
}
