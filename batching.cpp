//
// Created by Светлана Иванова on 19.04.2023.
//

#include "batching.h"
#include "mpz_helper.h"
#include <iostream>

const int MAX_CNT_BITS = 1000;

Batching::Batching(int _lambda, int _k, int _t, int _cnt, int _lambda_batch) {
    t = _t;
    cnt = _cnt;
    /// length of an input of PRF function
    int logcnt = 4;
    // int logcnt = std::bitset<MAX_CNT_BITS>(cnt).count();
    /// length of an output of PRF function
    int low_order = 3;
    helper = mpz_helper();
    prf = PRF(helper.get_random(_lambda_batch), _lambda_batch, logcnt, low_order);
    alpha.clear();
    alpha.resize(cnt);
}

void Batching::batch() {
    for (int i = 0; i < cnt; ++i) {
        std::cout << "new i: " << i << std::endl;
        alpha[i] = prf.gen(i);
    }
//    bigint batch_x = bigint(1);
//    bigint batch_y = bigint(1);
//    for (int i = 0; i < cnt; ++i) {
//        bigint xi = helper.pow(x[i], alpha[i], N);
//        bigint yi = helper.pow(y[i], alpha[i], N);
//        x = helper.mul_mod(x, xi, N);
//        y = helper.mul_mod(y, yi, N);
//    }

}

void Batching::print(std::ofstream& file) {
    for (int i = 0; i < cnt; ++i) {
        file << alpha[i].num << std::endl;
    }
}
