//
// Created by Светлана Иванова on 24.09.2023.
//

#ifndef SRC_TESTS_H
#define SRC_TESTS_H

#include "mpz_helper.h"

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
    int lambda, t, k;
    bigint x, N, y;
public:
    bool run();
    wesolowski_tests();
};

#endif //SRC_TESTS_H
