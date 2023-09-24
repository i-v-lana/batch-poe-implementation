#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gmp.h>


#include "wesolowski.h"
#include "batching.h"
#include "tests.h"
#include "PRF_AES.h"

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
    k = 7;

    bigint big_k = k;
    std::string iv = "000";
    std::cout << iv << std::endl;
    bigint big_x = 10;
    PRF_AES prf = PRF_AES(big_k, iv, 4);
    prf.evaluate(big_x);



    t = pow(2, 4);
    lambda = 8192;
    k = 3;
    w = 0;
    bigint p = bigint(31);
    bigint q = bigint(19);

    srand(time(NULL));

    WesolowskiParams w_params;
    w_params.k = k;

    Batching batch = Batching(w_params, t, 3, 10, bigint(589));
    batch.set_trapdoor(p, q);
    batch.batch();
//    batch.evaluate();
//    if (w == 0) {
//        result_verif = vdf.naive_verify(x, pow(2, t), l, pi);
//    }
//    else if(w == -1)
//    {
//        result_verif = vdf.parallel_verify(x, pow(2, t), l, pi);
//    }
//    else if(w == -2)
//    {
//        result_verif = vdf.parallel_diff_verify(x, pow(2, t), l, pi);
//    }
//    else
//    {
//        result_verif = vdf.optimized_verify(x, pow(2, t), l, pi, w);
//    }

    std::ofstream file;

    file.open("result/" + std::to_string(t) + "_" +
              std::to_string(lambda) + "_" + std::to_string(k)+ "_" + std::to_string(w) + ".csv",
              std::ofstream::out | std::ofstream::app);

    // batch.print(file);

//    file << vdf.setup_time.count() << ";" << vdf.eval_time.count() << ";"
//         << vdf.proof_time.count() << ";" << vdf.verif_time.count() << "\n";
//    std::cout << vdf.verif_time.count() << std::endl;

    file.close();
    return 0;
}