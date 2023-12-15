//
// Created by Светлана Иванова on 19.04.2023.
//

#ifndef SRC_BATCHING_H
#define SRC_BATCHING_H


#include <vector>
#include <gmp.h>
#include "wesolowski.h"
#include "mpz_helper.h"
#include <fstream>
#include "PRF_crypto.h"

struct WesolowskiParams {
    int k;
};

struct BatchingParams {
    long t;
    int cnt;
    int lambda_prf;
    bigint N;
    int low_order_bits;
    bigint low_order;
};

struct BatchingResult {
    std::vector<bigint> batch_x;
    std::vector<bigint> batch_y;
    bool result;
    std::chrono::duration<double> time;
};

class Batching {
protected:
    std::string iv = "ivnone";
    PRF_crypto prf = PRF_crypto(bigint(), iv, 128);
    bigint p, q;
    WesolowskiParams w_params;
    BatchingParams b_params;
    std::vector<bigint> alpha;
    mpz_helper helper;
    std::vector<bigint> x, y;
    void gen();
    void init(WesolowskiParams _w_params, BatchingParams _b_params);
    void set_trapdoor(bigint& _p, bigint& _q);
    bigint trapdoor(bigint& _x);

    void batch_prover_part(bigint* _l, bigint* _pi, bigint batch_x);
    std::pair<bool, std::chrono::duration<double>> batch_verifier_part(bigint& batch_x, bigint& batch_y, bigint& _l, bigint& _pi);
public:
    Batching(WesolowskiParams _w_params, BatchingParams _b_params, std::pair<bigint&, bigint&> _trapdoor);
    Batching(WesolowskiParams _w_params, BatchingParams _b_params, std::pair<std::vector<bigint&>, std::vector<bigint&>> xy, std::pair<bigint, bigint> _trapdoor);
    void print(std::ofstream& file);
    void print_cout();
    std::pair<std::vector<bigint>, std::vector<bigint> > get_instances();
    BatchingResult batch();
};


#endif //SRC_BATCHING_H
