//
// Created by Светлана Иванова on 24.09.2023.
//

#include "tests.h"
#include "wesolowski.h"
#include "PRF_crypto.h"
#include "batching.h"

bool tests::run() {
    prf_tests prf = prf_tests();
    batching_tests batching = batching_tests();
    wesolowski_tests wesolowski = wesolowski_tests();

    bool prf_result = prf.run();
    bool batching_result = batching.run();
    bool wesolowski_result = wesolowski.run();
    return prf_result && batching_result && wesolowski_result;
}

tests::tests() = default;

bool batching_tests::run() {
    bool small_result = small_test();
    bool normal_result = normal_test();
    bool result = small_result && normal_result;
    if (result) {
        std::cout << "TESTS: batching_tests were finished successfully" << std::endl;
    } else {
        std::cout << "TESTS: batching_tests failed." << std::endl;
    }
    return (small_result && normal_result);
}

bool batching_tests::small_test() {
    /// x ^ (2 ^ 16)
    long t = pow(2, 4);
    bigint p = bigint(31);
    bigint q = bigint(19);

    srand(time(NULL));

    WesolowskiParams w_params;
    /// l <- prime(1..2^(2*3))
    w_params.k = 6;
    BatchingParams b_params;
    b_params.t = t;
    /// PRF returns 3 bits numbers
    b_params.low_order_bits = 3;
    /// and takes 20 bits key.
    b_params.lambda_prf = 20;
    b_params.N = bigint(589);
    b_params.cnt = 10;


    Batching batch = Batching(w_params, b_params, {p, q});
    BatchingResult batch_result = batch.batch();
    bool result = (batch_result.batch_x == bigint(418)) && (batch_result.batch_y == bigint(171)) && batch_result.result;
    if (!result) {
        std::cout << "TESTS: simple batching_test failed." << std::endl;
    }
    return result;
}

bool batching_tests::normal_test() {
    /// x ^ (2 ^ 1024)
    long t = pow(2, 10);
    mpz_helper helper = mpz_helper();
    bigint p = helper.generate_prime(1024);
    bigint q = helper.generate_prime(1024);
    bigint N = p * q;
    std::cout << "p = " << p << std::endl;
    std::cout << "q = " << q << std::endl;
    std::cout << "N = " << N << std::endl;

    srand(time(NULL));

    WesolowskiParams w_params;
    /// l <- prime(1..2^(2*3))
    w_params.k = 1024;
    BatchingParams b_params;
    b_params.t = t;
    /// PRF returns 128 bits numbers
    b_params.low_order_bits = 128;
    /// and takes 128 bits key.
    b_params.lambda_prf = 128;
    b_params.N = N;
    b_params.cnt = 10;


    Batching batch = Batching(w_params, b_params, {p, q});
    BatchingResult batch_result = batch.batch();
    batch.print_cout();
    bool result = (batch_result.batch_x == bigint(418)) && (batch_result.batch_y == bigint(171)) && batch_result.result;
    if (!result) {
        std::cout << "TESTS: batching_test failed." << std::endl;
    }
    return result;
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
    if (prover_test) {
        std::cout << "TESTS: wesolowski prover test finished successfully." << std::endl;
    } else {
        std::cout << "TESTS: wesolowski prover test failed. Expected pi = " << check_pi << "; ";
        std::cout << "but received pi = " << pi << std::endl;
        std::cout << "TESTS: failed test parameters: x = " << x << "; " << "t = " << t << "; l = " << l << "; N = " << N << std::endl;
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

    if ((check_y == y.get_num()) == verify_result) {
        std::cout << "TESTS: wesolowski verifier test finished successfully." << std::endl;
        return true;
    } else {
        std::cout << "TESTS: wesolowski verifier test failed." << std::endl;
        std::cout << "TESTS: failed test parameters: x = " << x << "; " << "t = " << t << "; l = " << l << "; N = " << N << "; pi = " << pi << std::endl;
        return false;
    }
}

bool prf_tests::run() {
    bool result = true;
    result = result && test(bigint(260), "IV000", 128, bigint(58), bigint(58));
    result = result && test_cut(bigint(260), "IV000", 10, bigint(58), bigint(303));
    result = result && test(bigint(412), "IV000", 128, bigint(1038), bigint(1038));
    if (result) {
        std::cout << "TESTS: prf_tests finished successfully." << std::endl;
    } else {
        std::cout << "TESTS: prf_tests failed." << std::endl;
    }
    return result;
}

bool prf_tests::test(bigint _k, std::string _iv, int _output_bits, bigint _x, bigint _result) {
    PRF_crypto prf = PRF_crypto(_k, _iv, _output_bits);
    bigint encrypted = prf.evaluate(_x);
    bigint decrypted = prf.decrypt(encrypted);
    bool result = (decrypted == _result);
    if (result) return result;
    std::cout << "TESTS: prf_test failed for k = " << _k << "; x = " << _x << ". Expected result was " << _result << std::endl;
    return result;
}

bool prf_tests::test_cut(bigint _k, std::string _iv, int _output_bits, bigint _x, bigint _result) {
    PRF_crypto prf = PRF_crypto(_k, _iv, _output_bits);
    bigint encrypted = prf.evaluate(_x);
    bool result = (encrypted == _result);
    if (result) return result;
    std::cout << "TESTS: prf_test failed for k = " << _k << "; x = " << _x << "; output_len = " << _output_bits << ". Expected result was " << _result << std::endl;
    return result;
}

prf_tests::prf_tests() = default;
