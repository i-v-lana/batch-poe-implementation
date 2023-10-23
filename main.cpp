#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gmp.h>


#include "wesolowski.h"
#include "batching.h"
#include "tests.h"
#include "NaiveApproach.h"


void run_comparison() {
    std::cout << "=======STARTING COMPARISON=======" << std::endl;
    long t = pow(2, 9);
    mpz_helper helper = mpz_helper();
    bigint p = helper.generate_prime(2048);
    bigint q = helper.generate_prime(1024);
    bigint N = p * q;

    srand(time(NULL));

    WesolowskiParams w_params;
    /// l <- prime(1..2^2048)
    /// k bylo 1024, melo by byt 128.
    w_params.k = 128;
    BatchingParams b_params;
    b_params.t = t;
    /// PRF returns 128 bits numbers
    b_params.low_order_bits = 128;
    /// and takes 128 bits key.
    b_params.lambda_prf = 128;
    b_params.N = N;
    b_params.cnt = 1000;


    Batching batch = Batching(w_params, b_params, {p, q});
    BatchingResult batch_result = batch.batch();
    std::cout << "BATCHING RESULT IS " << batch_result.result << std::endl;
    std::pair<std::vector<bigint>, std::vector<bigint> > xy = batch.get_instances();
    NaiveApproach naive = NaiveApproach();
    naive.naive_approach(xy.first, xy.second, w_params, N, t);
    std::cout << "=======COMPARISON FINISHED=======" << std::endl;
}

// argv arguments :
//    t : log2 of difficulty (must be an integer)
//    lambda : length of modulus
//    k : bit-level security of hashing function
//    Size of Lenstra window w
int main(int argc, char *argv[]) {

    // Argument parsing
    int t, lambda, k, w;
//    int t = std::atoi(argv[1]);
//    int lambda = std::atoi(argv[2]);
//    int k = std::atoi(argv[3]);
//    int w = std::atoi(argv[4]);
    tests::run();
    for (int i = 0; i < 10; ++i) {
        run_comparison();
    }

//    std::ofstream file;
//
//    file.open("result/" + std::to_string(t) + "_" +
//              std::to_string(lambda) + "_" + std::to_string(k)+ "_" + std::to_string(w) + ".csv",
//              std::ofstream::out | std::ofstream::app);

    // batch.print(file);

//    file << vdf.setup_time.count() << ";" << vdf.eval_time.count() << ";"
//         << vdf.proof_time.count() << ";" << vdf.verif_time.count() << "\n";
//    std::cout << vdf.verif_time.count() << std::endl;

    //file.close();
    return 0;
}