//
// Created by Светлана Иванова on 24.09.2023.
//

#include "NaiveApproach.h"
#include "wesolowski.h"

void NaiveApproach::init(std::vector<bigint>& _x, std::vector<bigint>& _y,
                         WesolowskiParams _w_params, bigint& in_N, long _t) {
    l.clear();
    pi.clear();
    x = _x;
    y = _y;
    N = in_N;
    t = _t;
    w_params = _w_params;
    cnt = (int) x.size();
    l.resize(cnt);
    pi.resize(cnt);
}

bool NaiveApproach::check_trapdoor(bigint &_x) {
    mpz_helper helper = mpz_helper();
    bigint gcd = helper.gcd(_x, N);
    return (gcd == bigint(1));
}

void NaiveApproach::naive_approach() {
    naive_prover();
    naive_verifier();
}

NaiveApproach::NaiveApproach(std::vector<bigint>& _x, std::vector<bigint>& _y, WesolowskiParams _w_params, bigint& in_N, long _t) {
    init(_x, _y, _w_params, in_N, _t);
}

void NaiveApproach::set_trapdoor(bigint &_p, bigint &_q) {
    if (N == _p * _q) {
        p = _p;
        q = _q;
        trapdoor = true;
        std::cout << "Naive approach: trapdoor was set." << std::endl;
    } else {
        std::cout << "Naive approach: trapdoor wasn't set." << std::endl;
    }
}

void NaiveApproach::naive_prover() {
    for (int i = 0; i < cnt; ++i) {
        Wesolowski vdf = Wesolowski();
        vdf.setup(w_params.k, N.num);
        if (trapdoor && check_trapdoor(x[i]))
            vdf.prover_trapdoor(l[i].num, pi[i].num, x[i].num, t, ((p - 1UL) * (q - 1UL)).num);
        else
            vdf.prover(l[i].num, pi[i].num, x[i].num, t);
    }
}

void NaiveApproach::naive_verifier() {
    bool result = true;
    std::vector<bool> results(cnt, false);
    auto naive_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < cnt; ++i) {
        Wesolowski vdf = Wesolowski();
        vdf.setup(w_params.k, N.num);
        results[i] = vdf.verifier(x[i].num, y[i].num, t, l[i].num, pi[i].num);
        result = result && results[i];
    }
    auto naive_finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> naive_time = naive_finish - naive_start;
    std::cout << "NAIVE APPROACH: Total time of the verifier is " << naive_time.count() << std::endl;
}

NaiveApproach::NaiveApproach() = default;
