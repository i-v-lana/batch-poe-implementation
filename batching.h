//
// Created by Светлана Иванова on 19.04.2023.
//

#ifndef SRC_BATCHING_H
#define SRC_BATCHING_H


#include <vector>
#include <gmp.h>
#include "wesolowski.h"
#include "mpz_helper.h"
#include "PRF.h"
#include <fstream>

struct WesolowskiParams {
    int k;
};

class Batching {
private:
    PRF prf = PRF(bigint(), 0, 4, 0);
    int cnt;
    long t;
    bigint N, p, q;
    WesolowskiParams w_params;
    std::vector<bigint> alpha;
    mpz_helper helper;
    std::vector<bigint> x, y;
    void gen();
    void batch_prover_part(bigint* _l, bigint* _pi, bigint batch_x);
    std::pair<bool, long long> batch_verifier_part(bigint batch_x, bigint batch_y, bigint _l, bigint _pi);
public:
    void set_trapdoor(bigint& _p, bigint& _q);
    Batching(WesolowskiParams _w_params, long _t, int _cnt, int _lambda_batch, bigint _N);
    void print(std::ofstream& file);
    void batch();
    bigint trapdoor(bigint& _x);
};


#endif //SRC_BATCHING_H
