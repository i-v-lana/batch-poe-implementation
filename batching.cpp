//
// Created by Светлана Иванова on 19.04.2023.
//

#include "batching.h"
#include "mpz_helper.h"
#include "helper.h"
#include <iostream>

void Batching::init(WesolowskiParams _w_params, BatchingParams _b_params) {
    b_params = _b_params;
    w_params = _w_params;
    helper = mpz_helper();
    std::string _iv = "IVinput";
    prf = PRF_crypto(helper.get_random(b_params.lambda_prf), _iv, b_params.low_order_bits);
}

Batching::Batching(WesolowskiParams &_w_params, BatchingParams &_b_params, std::pair<bigint, bigint> _trapdoor) {
    init(_w_params, _b_params);
    set_trapdoor(_trapdoor.first, _trapdoor.second);
    gen();
}

Batching::Batching(WesolowskiParams &_w_params, BatchingParams &_b_params, std::pair<std::vector<bigint>, std::vector<bigint>> &xy, std::pair<bigint, bigint> _trapdoor) {
    init(_w_params, _b_params);
    x.clear();
    y.clear();
    x.resize(b_params.cnt);
    y.resize(b_params.cnt);
    for (int i = 0; i < b_params.cnt; ++i) {
        x[i] = xy.first[i];
        y[i] = xy.second[i];
    }
    set_trapdoor(_trapdoor.first, _trapdoor.second);
}

BatchingResult Batching::combine() {
    alpha.clear();
    alpha.resize(b_params.cnt);
    for (int i = 0; i < b_params.cnt; ++i) {
        bigint cur_i = bigint(i);
        alpha[i] = prf.evaluate(cur_i);
    }
    bigint batch_x = bigint(1);
    bigint batch_y = bigint(1);
    auto start_total = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < b_params.cnt; ++i) {
        bigint xi = helper.pow(x[i], alpha[i], b_params.N);
        bigint yi = helper.pow(y[i], alpha[i], b_params.N);
        helper.mul_mod(batch_x, batch_x, xi, b_params.N);
        helper.mul_mod(batch_y, batch_y, yi, b_params.N);
    }
    auto end_total = std::chrono::high_resolution_clock::now();
    BatchingResult combine_result;
    combine_result.time = (end_total - start_total);
    combine_result.batch_x = {batch_x};
    combine_result.batch_y = {batch_y};
    combine_result.result = true;
    return combine_result;
}

BatchingResult Batching::batch() {
    BatchingResult batch_result = combine();
    bigint _l = bigint();
    bigint _pi = bigint();
    batch_prover_part(&_l, &_pi, batch_result.batch_x[0]);
    std::pair<bool, std::chrono::duration<double>> result = batch_verifier_part(batch_result.batch_x[0], batch_result.batch_y[0], _l, _pi);

    auto total_lior_rothem_time = batch_result.time + result.second;

    batch_result.result = result.first;
    batch_result.time = total_lior_rothem_time;
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

std::pair<std::vector<bigint>, std::vector<bigint> > Batching::get_instances() {
    return {x, y};
}

bigint Batching::trapdoor(bigint& _x) {
    if (!trapdoor_flag) {
        print_error("BATCHING: Can't calculate the trapdoor, as it wasn't correctly set");
        return _x;
    }
    if (!check_trapdoor(_x)) {
        print_error("BATCHING: input for trapdoor function isn't coprime with module");
        return _x;
    }
    /// N = pq -> phi = (p - 1) * (q - 1)
    bigint phi = (p - 1UL) * (q - 1UL);
    /// counting 2^t mod phi
    bigint two(2);
    bigint big_t(b_params.t);
    bigint power = helper.pow(two, big_t, phi);
    bigint ans = helper.pow(_x, power, b_params.N);
    return ans;
}

void Batching::set_trapdoor(bigint& _p, bigint& _q) {
    if (b_params.N == _p * _q) {
        p = _p;
        q = _q;
        trapdoor_flag = true;
    } else {
        trapdoor_flag = false;
        print_error("BATCHING: trapdoor wasn't set");
    }
}

void Batching::batch_prover_part(bigint* _l, bigint* _pi, bigint& batch_x) {
    Wesolowski vdf = Wesolowski();
    vdf.setup(w_params.k, b_params.N.num, b_params.t);
    if (trapdoor_flag)
        vdf.prover_trapdoor(_l->num, _pi->num, batch_x.num, ((p - 1UL) * (q - 1UL)).num);
    else
        vdf.prover(_l->num, _pi->num, batch_x.num);
}

std::pair<bool, std::chrono::duration<double>> Batching::batch_verifier_part(bigint& batch_x, bigint& batch_y, bigint& _l, bigint& _pi) {
    auto wes_start = std::chrono::high_resolution_clock::now();
    Wesolowski vdf = Wesolowski();
    vdf.setup(w_params.k, b_params.N.num, b_params.t);
    bool result = vdf.verifier(batch_x.num, batch_y.num, _l.num, _pi.num);
    auto wes_end = std::chrono::high_resolution_clock::now();
    return std::make_pair(result, (wes_end - wes_start));
}

bool Batching::check_trapdoor(bigint &_x) {
    bigint gcd = helper.gcd(_x, b_params.N);
    return (gcd == bigint(1));
}

