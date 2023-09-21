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

class Batching {
private:
    PRF prf = PRF(bigint(), 0, 4, 0);
    int cnt;
    int t;
    int lambda;
    int k;
    bigint N, p, q;
    std::vector<bigint> alpha;
    mpz_helper helper;
    std::vector<bigint> x, y, l, pi;
    void gen();
    void batch_prover_part(bigint* _l, bigint* _pi, bigint batch_x);
    std::pair<bool, long long> batch_verifier_part(bigint batch_x, bigint batch_y, bigint _l, bigint _pi);
    void naive_prover_part();
    void naive_verifier_part();
public:
    void set_trapdoor(bigint& _p, bigint& _q);
    Batching(int _lambda, int _k, int _t, int _cnt, int _lambda_batch, bigint _N);
    void naive_approach();
    void print(std::ofstream& file);
    void batch();
    bigint trapdoor(bigint& _x);
};


#endif //SRC_BATCHING_H
