#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gmp.h>
#include <algorithm>
#include <random>


#include "batching.h"
#include "GenInstances.h"
#include "helper.h"
#include "Experiments.h"


void run_comparison(runparams params, std::vector<bigint> x, std::vector<bigint> y) {
    print_info("STARTING COMPARISON");

    long long t = pow(2, params.logt);
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
    b_params.N = params.N;
    b_params.cnt = x.size();

    /// TODO: better write down the result of the experiments;
//    std::string file_name = "exp.csv";
//    std::ofstream text_file(file_name, std::ios::app);
//    text_file << params.logt << "," << x.size() << ",";
    BatchingResult result = run_protocol(w_params,
                                         b_params,
                                         {x, y},
                                         params.trapdoor,
                                         params.protocol,
                                         params.output_file);
//    std::cout << "the protocol running time is " << result.time.count() << std::endl;
//    text_file.close();
    print_info("COMPARISON FINISHED");
}


/// TODO: IMPORTAN - COMPARE THE ALL_BUT_NAIVE AND SEPARATE PROTOCOLS RUNNING TIMES - DO SOME TEST!
int main(int argc, char *argv[]) {
    /// argv[1] - protocol name (naive/bucket/hybrid/subset/exponential/all/all_but_naive)
    /// argv[2] - logT
    /// argv[3] - experiments number
    /// argv[4] - number of instances per experiment
    /// argv[5] (voluntary) - the path to the file for storing output data
    /// TODO: add arguments for the testing run
    /// TODO: add to the cmakelists.txt the compiler flags for optimization.
    errortype error;
    runparams params = get_runparams(argc, argv, error);
    if (error.error) {
        print_error(error.error_msg);
        return -1;
    }


    /// TODO: create a db with the instances; connect to the db from c++ to read the instances
    Experiments experiment(100, params.N, params.trapdoor, pow(2, params.logt));
    auto xys = experiment.get(params.instances_per_exp, params.experiments);
    for (auto&& xy : xys) {
        run_comparison(params, xy.first, xy.second);
    }
    return 0;
}