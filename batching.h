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
    std::vector<bigint> x, y;
    void gen();
    bool run_vdf(bigint cur_x, bigint cur_y);
public:
    void set_trapdoor(bigint& _p, bigint& _q);
    Batching(int _lambda, int _k, int _t, int _cnt, int _lambda_batch, bigint _N);
    void print(std::ofstream& file);
    void batch();
    bigint trapdoor(bigint x);
};


#endif //SRC_BATCHING_H
