//
// Created by Светлана Иванова on 19.04.2023.
//

#include "batching.h"
#include "mpz_helper.h"
#include <iostream>

void Batching::init(WesolowskiParams _w_params, BatchingParams _b_params) {
    b_params = _b_params;
    w_params = _w_params;
    helper = mpz_helper();
    std::string _iv = "IVinput";
    prf = PRF_crypto(helper.get_random(b_params.lambda_prf), _iv, b_params.low_order_bits);
}

Batching::Batching(WesolowskiParams _w_params, BatchingParams _b_params, std::pair<bigint, bigint> _trapdoor) {
    init(_w_params, _b_params);
    set_trapdoor(_trapdoor.first, _trapdoor.second);
    gen();
}

Batching::Batching(WesolowskiParams _w_params, BatchingParams _b_params, std::vector<std::pair<bigint, bigint>> xy) {
    init(_w_params, _b_params);
    x.clear();
    y.clear();
    x.resize(b_params.cnt);
    y.resize(b_params.cnt);
    for (int i = 0; i < b_params.cnt; ++i) {
        x[i] = xy[i].first;
        y[i] = xy[i].second;
    }
}

BatchingResult Batching::batch() {
    auto start_batching = std::chrono::high_resolution_clock::now();
    alpha.clear();
    alpha.resize(b_params.cnt);
    for (int i = 0; i < b_params.cnt; ++i) {
        bigint cur_i = bigint(i);
        alpha[i] = prf.evaluate(cur_i);
    }
    bigint batch_x = bigint(1);
    bigint batch_y = bigint(1);
    for (int i = 0; i < b_params.cnt; ++i) {
        bigint xi = helper.pow(x[i], alpha[i], b_params.N);
        bigint yi = helper.pow(y[i], alpha[i], b_params.N);
        batch_x = helper.mul_mod(batch_x, xi, b_params.N);
        batch_y = helper.mul_mod(batch_y, yi, b_params.N);
    }
    auto end_batching = std::chrono::high_resolution_clock::now();
    bigint _l = bigint();
    bigint _pi = bigint();
    batch_prover_part(&_l, &_pi, batch_x);
    std::pair<bool, std::chrono::duration<double>> result = batch_verifier_part(batch_x, batch_y, _l, _pi);
    std::chrono::duration<double> lior_rothem_time = end_batching - start_batching + result.second;
    std::cout << "Total time of the Lior Rothem's protocol: " << lior_rothem_time.count() << std::endl;
    BatchingResult batch_result;
    batch_result.result = result.first;
    batch_result.batch_x = batch_x;
    batch_result.batch_y = batch_y;
    return batch_result;
}

void Batching::print(std::ofstream& file) {
    for (int i = 0; i < b_params.cnt; ++i) {
        file << x[i].num << " " << y[i].num << std::endl;
        file << alpha[i].num << std::endl;
    }
}

void Batching::print_cout() {
    for (int i = 0; i < b_params.cnt; ++i) {
        std::cout << "x = " << x[i].num << "; y = " << y[i].num << std::endl;
        std::cout << "alpha[i] = " << alpha[i].num << std::endl;
    }
}

void Batching::gen() {
    x.clear();
    y.clear();
    x.resize(b_params.cnt);
    y.resize(b_params.cnt);
    for (int i = 0; i < b_params.cnt; ++i) {
        x[i] = helper.get_random_mod(b_params.N);
        y[i] = trapdoor(x[i]);
    }
}

bigint Batching::trapdoor(bigint& _x) {
    /// N = pq -> phi = (p - 1) * (q - 1)
    bigint phi = (p - 1UL) * (q - 1UL);
    /// counting 2^t mod phi
    bigint power = helper.pow(bigint(2), bigint(b_params.t), phi);
    bigint ans = helper.pow(_x, power, b_params.N);
    return ans;
}

void Batching::set_trapdoor(bigint& _p, bigint& _q) {
    if (b_params.N == _p * _q) {
        p = _p;
        q = _q;
        std::cout << "BATCHING: trapdoor was set." << std::endl;
    } else {
        std::cout << "BATCHING: trapdoor wasn't set." << std::endl;
    }
}

void Batching::batch_prover_part(bigint* _l, bigint* _pi, bigint batch_x) {
    Wesolowski vdf = Wesolowski();
    vdf.setup(w_params.k, b_params.N.num);
    vdf.prover(_l->num, _pi->num, batch_x.num, b_params.t);
}

std::pair<bool, std::chrono::duration<double>> Batching::batch_verifier_part(bigint batch_x, bigint batch_y, bigint _l, bigint _pi) {
    Wesolowski vdf = Wesolowski();
    vdf.setup(w_params.k, b_params.N.num);
    auto wes_start = std::chrono::high_resolution_clock::now();
    bool result = vdf.verifier(batch_x.num, batch_y.num, b_params.t, _l.num, _pi.num);
    auto wes_end = std::chrono::high_resolution_clock::now();
    return std::make_pair(result, (wes_end - wes_start));
}

