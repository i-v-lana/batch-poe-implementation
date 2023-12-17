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
#include "HybridBatching.h"
#include "BucketBatching.h"
#include "GenInstances.h"


void run_comparison(bigint N, std::pair<bigint, bigint> trapdoor, long long t, std::vector<bigint> x, std::vector<bigint> y) {
    std::cout << "=======STARTING COMPARISON=======" << std::endl;
//    generator.gen(10000000, "instances.txt");
//    generator.get_instances("instances.txt", 1000);


    WesolowskiParams w_params;
    /// l <- prime(1..2^(2k))
    /// k bylo 1024, melo by byt 128.
    w_params.k = 128;
    BatchingParams b_params;
    b_params.t = t;
    /// PRF returns 128 bits numbers - the length of the alpha is log^2(lambda)
    b_params.low_order_bits = 100;
    /// and takes 128 bits key.
    b_params.lambda_prf = 128;
    b_params.N = N;
    b_params.cnt = x.size();


    Batching batch = Batching(w_params, b_params, {x, y}, trapdoor);
    BatchingResult batch_result = batch.batch();
    std::cout << "BATCHING RESULT IS " << batch_result.result << std::endl;

    b_params.low_order_bits = 128;
    HybridBatching hybrid_batch = HybridBatching(w_params, b_params, {x, y}, trapdoor);
    BatchingResult hybrid_batch_result = hybrid_batch.batch(100);
    std::cout << "HYBRID BATCHING RESULT IS " << hybrid_batch_result.result << std::endl;

    BucketBatching bucket_batch = BucketBatching(w_params, b_params, {x, y}, trapdoor);
    BatchingResult bucket_batch_result = bucket_batch.batch(5);
    std::cout << "BUCKET BATCHING RESULT IS" << bucket_batch_result.result << std::endl;

    b_params.low_order_bits = 128;
    SubsetBatching subset_batch = SubsetBatching(w_params, b_params, {x, y}, trapdoor);
    BatchingResult subset_batch_result = subset_batch.batch(100);
    std::cout << "SUBSET BATCHING RESULT IS " << subset_batch_result.result << std::endl;


//    NaiveApproach naive = NaiveApproach(xy.first, xy.second, w_params, N, t);
//    naive.set_trapdoor(p, q);
//    naive.naive_approach();
    std::cout << "=======COMPARISON FINISHED=======" << std::endl;
}

int main(int argc, char *argv[]) {

    // Argument parsing
    long t = pow(2, 25);
    srand(time(NULL));
    long long cnt = 10;

    mpz_helper helper = mpz_helper();
    bigint p = helper.generate_prime(1024);
    bigint q = helper.generate_prime(1024);
    bigint N = p * q;

    GenInstances generator = GenInstances(N, {p, q}, t);
    auto xy = generator.get_instances("instances.txt", 10 * cnt);
//    int t = std::atoi(argv[1]);
//    int lambda = std::atoi(argv[2]);
//    int k = std::atoi(argv[3]);
//    int w = std::atoi(argv[4]);
//    tests::run();
    for (int i = 0; i < 10; ++i) {
        std::vector<bigint> cur_x(xy.first.begin() + (long long)i * cnt, xy.first.begin() + (long long)(i + 1) * cnt);
        std::vector<bigint> cur_y(xy.second.begin() + (long long)i * cnt, xy.second.begin() + (long long)(i + 1) * cnt);
        run_comparison(N, {p, q}, t, cur_x, cur_y);
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