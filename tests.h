//
// Created by Светлана Иванова on 24.09.2023.
//

#ifndef SRC_TESTS_H
#define SRC_TESTS_H

#include "mpz_helper.h"
#include "wesolowski.h"

class tests {
public:
    static bool run();
    tests();
};

class batching_tests : tests {
private:
    bool small_test();
    bool normal_test();
public:
    bool run();
    batching_tests();
};

class wesolowski_tests : tests {
private:
    int t, k;
    bigint x, N, y;
    Wesolowski vdf;
    bool prover_test(bigint& l, bigint& pi);
    bool trapdoor_prover_test(bigint &l, bigint &pi, bigint _p, bigint _q);
    bool verifier_test(bigint l, bigint pi);
public:
    bool run();
    wesolowski_tests();
};

class prf_tests : tests {
private:
    bool test(bigint _k, std::string _iv, int _output_bits, bigint _x, bigint _result);
    bool test_cut(bigint _k, std::string _iv, int _output_bits, bigint _x, bigint _result);
public:
    bool run();
    prf_tests();
};

#endif //SRC_TESTS_H
