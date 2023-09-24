//
// Created by Светлана Иванова on 24.09.2023.
//

#include "tests.h"
#include "wesolowski.h"

bool tests::run() {
    batching_tests batching = batching_tests();
    wesolowski_tests wesolowski = wesolowski_tests();
    bool batching_result = batching.run();
    bool wesolowski_result = wesolowski.run();
    return batching_result && wesolowski_result;
}

tests::tests() = default;

bool batching_tests::run() {

    return false;
}

batching_tests::batching_tests() = default;

bool wesolowski_tests::run() {
    bool tests_result = true;
    vdf.setup(k, N.num);

    /// test for prover
    bigint l = bigint();
    bigint pi = bigint();
    tests_result = tests_result && prover_test(l, pi);

    /// test for verifier
    tests_result = tests_result && verifier_test(l, pi);
    return tests_result;
}

wesolowski_tests::wesolowski_tests() {
    t = 5;
    k = 2; /// l will be from 1 to 16
    N = 5 * 7; /// 3-bits primes
    x = 14;
    y = 21; /// 16^32 mod 35 = 11
    vdf = Wesolowski();
}

bool wesolowski_tests::prover_test(bigint &l, bigint &pi) {
    vdf.prover(l.num, pi.num, x.num, t);

    unsigned long int exp = pow(2, t);
    exp = exp / l.get_num();
    unsigned long int check_pi = pow(x.get_num(), exp);
    check_pi = check_pi % N.get_num();

    bool prover_test = (check_pi == pi.get_num());
    std::cout << "TESTS: running wesolowski prover test for x = " << x << "; " << "t = " << t << "; l = " << l << "; N = " << N << std::endl;
    if (prover_test) {
        std::cout << "TESTS: wesolowski prover test finished successfully, pi = " << pi << "\n";
    } else {
        std::cout << "TESTS: wesolowski prover test failed. Expected pi = " << check_pi << "; ";
        std::cout << "but received pi = " << pi << std::endl;
    }
    return prover_test;
}

bool wesolowski_tests::verifier_test(bigint l, bigint pi) {
    bool verify_result = vdf.verifier(x.num, y.num, t, l.num, pi.num);
    unsigned long int exp = pow(2, t);
    unsigned long int r = exp % l.get_num();
    unsigned long int xr = pow(x.get_num(), r);
    xr = xr % N.get_num();
    unsigned long int pi_l = pow(pi.get_num(), l.get_num());
    pi_l = pi_l % N.get_num();
    unsigned long int check_y = (xr * pi_l) % N.get_num();
    std::cout << "TESTS: running wesolowski verifier test for x = " << x << "; " << "t = " << t << "; l = " << l << "; N = " << N << "; pi = " << pi << std::endl;

    if ((check_y == y.get_num()) == verify_result) {
        std::cout << "TESTS: wesolowski verifier test finished successfully, y = " << check_y << "\n";
        return true;
    } else {
        std::cout << "TESTS: wesolowski verifier test failed." << std::endl;
        return false;
    }
}