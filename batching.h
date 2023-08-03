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
    bigint N;
    std::vector<bigint> alpha;
    mpz_helper helper;
    std::vector<bigint> x, y;
    void gen();
public:
    Batching(int _lambda, int _k, int _t, int _cnt, int _lambda_batch, bigint _N);
    void print(std::ofstream& file);
    void batch();
};


#endif //SRC_BATCHING_H
