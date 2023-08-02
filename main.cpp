#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gmp.h>


#include "wesolowski.h"
#include "batching.h"

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

    t = 6;
    lambda = 8192;
    k = 128;
    w = 0;

    srand(time(NULL));

    Batching batch = Batching(lambda, k, t, 3);
    batch.evaluate();
    if (w == 0) {
        result_verif = vdf.naive_verify(x, pow(2, t), l, pi);
    }
    else if(w == -1)
    {
        result_verif = vdf.parallel_verify(x, pow(2, t), l, pi);
    }
    else if(w == -2)
    {
        result_verif = vdf.parallel_diff_verify(x, pow(2, t), l, pi);
    }
    else
    {
        result_verif = vdf.optimized_verify(x, pow(2, t), l, pi, w);
    }

    std::ofstream file;

    file.open("result/" + std::to_string(t) + "_" +
              std::to_string(lambda) + "_" + std::to_string(k)+ "_" + std::to_string(w) + ".csv",
              std::ofstream::out | std::ofstream::app);

    batch.print(file);

//    file << vdf.setup_time.count() << ";" << vdf.eval_time.count() << ";"
//         << vdf.proof_time.count() << ";" << vdf.verif_time.count() << "\n";
//    std::cout << vdf.verif_time.count() << std::endl;

    file.close();
    return 0;
}