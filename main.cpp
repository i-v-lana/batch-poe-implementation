#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gmp.h>
#include <algorithm>
#include <random>


#include "wesolowski.h"
#include "batching.h"
#include "tests.h"
#include "NaiveApproach.h"
#include "SubsetBatching.h"
#include "HybridBatching.h"
#include "BucketBatching.h"
#include "GenInstances.h"


void run_comparison(bigint N, std::pair<bigint, bigint> trapdoor, long logt, std::vector<bigint> x, std::vector<bigint> y) {
    std::cout << "=======STARTING COMPARISON=======" << std::endl;
//    generator.gen(10000000, "instances.txt");
//    generator.get_instances("instances.txt", 1000);

    long long t = pow(2, logt);
    WesolowskiParams w_params;
    /// l <- prime(1..2^(2k))
    /// k bylo 1024, melo by byt 128.
    w_params.k = 128;
    BatchingParams b_params;
    b_params.t = t;
    /// TODO: think about 100 vs 128. PRF returns 128 bits numbers - the length of the alpha is log^2(lambda)
    b_params.low_order_bits = 128;
    /// and takes 128 bits key.
    b_params.lambda_prf = 128;
    b_params.N = N;
    b_params.cnt = x.size();

    std::string file_name = "exp.csv";
    std::ofstream text_file(file_name, std::ios::app);
    text_file << logt << "," << x.size() << ",";

//    Batching batch = Batching(w_params, b_params, {x, y}, trapdoor);
//    BatchingResult batch_result = batch.batch();
//    std::cout << "Total time of the rothem batching protocol: " << batch_result.time.count() << std::endl;
////    std::cout << "BATCHING RESULT IS " << batch_result.result << std::endl;
//    text_file << batch_result.time.count() << ",";
//
//    b_params.low_order_bits = 128;
//    HybridBatching hybrid_batch = HybridBatching(w_params, b_params, {x, y}, trapdoor);
//    BatchingResult hybrid_batch_result = hybrid_batch.batch(128);
////    std::cout << "HYBRID BATCHING RESULT IS " << hybrid_batch_result.result << std::endl;
//    text_file << hybrid_batch_result.time.count() << ",";
//
//    BucketBatching bucket_batch = BucketBatching(w_params, b_params, {x, y}, trapdoor);
//    BatchingResult bucket_batch_result = bucket_batch.batch(11);
////    std::cout << "BUCKET BATCHING RESULT IS " << bucket_batch_result.result << std::endl;
//    text_file << bucket_batch_result.time.count() << ",";

//    b_params.low_order_bits = 128;
    SubsetBatching subset_batch = SubsetBatching(w_params, b_params, {x, y}, trapdoor);
    BatchingResult subset_batch_result = subset_batch.batch(128);
    std::cout << "Total time of the random subset batching protocol: " << subset_batch_result.time.count() << std::endl;
//    std::cout << "SUBSET BATCHING RESULT IS " << subset_batch_result.result << std::endl;
    text_file << subset_batch_result.time.count() << "\n";


//    NaiveApproach naive = NaiveApproach(xy.first, xy.second, w_params, N, t);
//    naive.set_trapdoor(p, q);
//    naive.naive_approach();
    text_file.close();
    std::cout << "=======COMPARISON FINISHED=======" << std::endl;
}

int main(int argc, char *argv[]) {

    // Argument parsing
    long logt = 25;
    long t = pow(2, logt);
    srand(time(NULL));
    long long cnt = 5e5;

    mpz_helper helper = mpz_helper();
    bigint p = helper.generate_prime(1024);
    bigint q = helper.generate_prime(1024);
    bigint N = p * q;

    GenInstances generator = GenInstances(N, {p, q}, t);
    auto xy = generator.get_instances("instances.txt", 1 * cnt);
    std::vector<int> indecies;
    for (int i = 0; i < xy.first.size(); ++i) {
        indecies.push_back(i);
    }
    std::random_device rd;
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(indecies), std::end(indecies), rng);
    for (int i = 0; i < 1; ++i) {
        /// TODO: fix later
        std::vector<bigint> cur_x;
        std::vector<bigint> cur_y;
        for (auto j : indecies) {
            if (cur_x.size() == cnt) {
                break;
            }
            cur_x.emplace_back(xy.first[j]);
            cur_y.emplace_back(xy.second[j]);
        }
        run_comparison(N, {p, q}, logt, cur_x, cur_y);
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