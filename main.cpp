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
#include "SubsetBatching.h"


void run_comparison() {
    std::cout << "=======STARTING COMPARISON=======" << std::endl;
    long t = pow(2, 30);
    mpz_helper helper = mpz_helper();
    bigint p = helper.generate_prime(1024);
    bigint q = helper.generate_prime(1024);
    bigint N = p * q;

    srand(time(NULL));

    WesolowskiParams w_params;
    /// l <- prime(1..2^(2k))
    /// k bylo 1024, melo by byt 128.
    w_params.k = 128;
    BatchingParams b_params;
    b_params.t = t;
    /// PRF returns 128 bits numbers - the length of the alpha is log^2(lambda)
    b_params.low_order_bits = 49;
    /// and takes 128 bits key.
    b_params.lambda_prf = 128;
    b_params.N = N;
    b_params.cnt = 1000;


    Batching batch = Batching(w_params, b_params, {p, q});
    BatchingResult batch_result = batch.batch();
    std::cout << "BATCHING RESULT IS " << batch_result.result << std::endl;
    std::pair<std::vector<bigint>, std::vector<bigint> > xy = batch.get_instances();

    SubsetBatching subset_batch = SubsetBatching(w_params, b_params, xy, {p, q});
    BatchingResult subset_batch_result = subset_batch.batch(100);
    std::cout << "SUBSET BATCHING RESULT IS " << subset_batch_result.result << std::endl;


//    NaiveApproach naive = NaiveApproach(xy.first, xy.second, w_params, N, t);
//    naive.set_trapdoor(p, q);
//    naive.naive_approach();
    std::cout << "=======COMPARISON FINISHED=======" << std::endl;
}

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