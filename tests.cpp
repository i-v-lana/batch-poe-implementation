//
// Created by Светлана Иванова on 24.09.2023.
//

#include "tests.h"
#include "wesolowski.h"
#include "PRF_crypto.h"
#include "batching.h"

bool tests::run() {
    std::cout << "==========RUNNING TESTS==========" << std::endl;
    prf_tests prf = prf_tests();
    batching_tests batching = batching_tests();
    wesolowski_tests wesolowski = wesolowski_tests();

    bool prf_result = prf.run();
    bool batching_result = batching.run();
    bool wesolowski_result = wesolowski.run();
    bool result = prf_result && batching_result && wesolowski_result;

    if (result) {
        std::cout << "TESTS FINISHED SUCCESSFULLY" << std::endl;
    } else {
        std::cout << "TESTS FAILED." << std::endl;
    }
    std::cout << "============TESTS DONE============" << std::endl;
    return result;
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


    RandomExponents batch = RandomExponents(w_params, b_params, {p, q});
    BatchingResult batch_result = batch.batch();
    bool result = (batch_result.batch_x[0] == bigint(418)) && (batch_result.batch_y[0] == bigint(171)) && batch_result.result;
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


    RandomExponents batch = RandomExponents(w_params, b_params, {p, q});
    BatchingResult batch_result = batch.batch();
    const char *test_result_x = "188815551064059700799729473638480500879704544260"
            "860773062472340755036998819289877257449822956749293719208991079293780"
            "608626585242259950666122239791134100918325915369222931830813630937169"
            "805174129552820609916519656954547374052563296038444453425446047231069"
            "774629529990947471353105629137680007762147650585011247828653963668531"
            "004833495890272297967261210031599202480036859816508975644477018838946"
            "728142462429139994915339388517338411055569526462229290423419825456905"
            "757579865525877074827736787434320414205756606321595861603586337576365"
            "32169655554133822726094754191369361965972537760421698825463568864168376636927420384537";
    const char *test_result_y = "1004635983377887160846782723975916975445790330290"
                                "2155097593479803735437316869296741403915627475322"
                                "5092791326125437879598385901885759555450117201376"
                                "5063183624830219106877197573149511974104079140398"
                                "2167454373612155095208669329812904913623744429630"
                                "9880853406737850400275885724760759782920798935251"
                                "6201034036062547711918259961312688839220081170280"
                                "1957690478337459285669823959489177132015734500898"
                                "8341621252090935628396216411793649486265003207158"
                                "0364145375036193172949585497022064443827483067452"
                                "3392310178123346686692107318535881373905873623626"
                                "750898736448925351799860500691484207972181421910473032907301257384991553350801";
    bool result = (batch_result.batch_x[0] == bigint(test_result_x))
            && (batch_result.batch_y[0] == bigint(test_result_y)) && batch_result.result;
    if (!result) {
        std::cout << "TESTS: batching_test failed." << std::endl;
    }
    return result;
}

batching_tests::batching_tests() = default;

bool wesolowski_tests::run() {
    bool tests_result = true;
    vdf.setup(k, N.num, t);

    /// test for prover
    bigint l = bigint();
    bigint pi = bigint();
    tests_result = tests_result && prover_test(l, pi) && trapdoor_prover_test(l, pi, bigint(5), bigint(11));

    /// test for verifier
    tests_result = tests_result && verifier_test(l, pi);
    return tests_result;
}

wesolowski_tests::wesolowski_tests() {
    t = 5;
    k = 2; /// l will be from 1 to 16
    N = bigint(5 * 11); /// 3-bits primes
    x = bigint(14);
    y = bigint(31); /// 16^32 mod 35 = 11
    vdf = Wesolowski();
}

bool wesolowski_tests::prover_test(bigint &l, bigint &pi) {
    vdf.prover(l.num, pi.num, x.num);

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


bool wesolowski_tests::trapdoor_prover_test(bigint &l, bigint &pi, bigint _p, bigint _q) {
    vdf.prover_trapdoor(l.num, pi.num, x.num, ((_p - 1UL) * (_q - 1UL)).num);

    unsigned long int exp = pow(2, t);
    exp = exp / l.get_num();
    unsigned long int check_pi = pow(x.get_num(), exp);
    check_pi = check_pi % N.get_num();

    bool prover_test = (check_pi == pi.get_num());
    if (prover_test) {
        std::cout << "TESTS: wesolowski trapdoor prover test finished successfully." << std::endl;
    } else {
        std::cout << "TESTS: wesolowski trapdoor prover test failed. Expected pi = " << check_pi << "; ";
        std::cout << "but received pi = " << pi << std::endl;
        std::cout << "TESTS: failed test parameters: x = " << x << "; " << "t = " << t << "; l = " << l << "; N = " << N << std::endl;
    }
    return prover_test;
}


bool wesolowski_tests::verifier_test(bigint l, bigint pi) {
    bool verify_result = vdf.verifier(x.num, y.num, l.num, pi.num);
    unsigned long int exp = pow(2, t);
    unsigned long int r = exp % l.get_num();
    unsigned long int xr = pow(x.get_num(), r);
    xr = xr % N.get_num();
    unsigned long long pi_l = mpz_helper().pow(pi, l, N).get_num();
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
