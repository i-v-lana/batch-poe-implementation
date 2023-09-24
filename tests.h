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
    bool verifier_test(bigint l, bigint pi);
public:
    bool run();
    wesolowski_tests();
};

#endif //SRC_TESTS_H
