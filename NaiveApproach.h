//
// Created by Светлана Иванова on 24.09.2023.
//

#ifndef SRC_NAIVEAPPROACH_H
#define SRC_NAIVEAPPROACH_H


#include "mpz_helper.h"
#include "batching.h"

class NaiveApproach {
private:
    std::vector<bigint> x, y, l, pi;
    int cnt = 0;
    void naive_prover();
    void naive_verifier();
    void init(std::vector<bigint>& _x, std::vector<bigint>& _y, WesolowskiParams _w_params, bigint& in_N, long _t);
    WesolowskiParams w_params;
    bigint N;
    long t;
public:
    void naive_approach(std::vector<bigint>& _x, std::vector<bigint>& _y, WesolowskiParams _w_params, bigint& in_N, long _t);
    NaiveApproach();
};


#endif //SRC_NAIVEAPPROACH_H
