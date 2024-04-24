//
// Created by Светлана Иванова on 09.04.2024.
//
#include "batching.h"
#include "helper.h"
#include "HybridBatching.h"
#include "BucketBatching.h"
#include "SubsetBatching.h"
#include "NaiveApproach.h"

void print_info(std::string info) {
    std::cout << "INFO: " << info << std::endl;
}

template <typename T>
void print_info(std::string info, T param) {
    std::cout << "INFO: " << info <<  " " << param << std::endl;
}

BatchingResult run_protocol(WesolowskiParams _w_params, BatchingParams _b_params,
                          std::pair<std::vector<bigint>, std::vector<bigint>> xy,
                          std::pair<bigint, bigint> _trapdoor, protocoltype _protocol) {
    BatchingResult result = {};
    switch(_protocol) {
        /// TODO: why 128? wrap it to the parameter? why 11?
        case hybrid: {
            _b_params.low_order_bits = 128;
            HybridBatching hybrid_batch = HybridBatching(_w_params, _b_params, xy, _trapdoor);
            result = hybrid_batch.batch(128);
            print_info("Running time of the hybrid protocol is", result.time.count());
            break;
        }
        case naive: {
            NaiveApproach naive_batch = NaiveApproach(_w_params, _b_params, xy, _trapdoor);
            result = naive_batch.batch();
            print_info("Running time of the naive protocol is", result.time.count());
            break;
        }
        case bucket: {
            BucketBatching bucket_batch = BucketBatching(_w_params, _b_params, xy, _trapdoor);
            result = bucket_batch.batch(11);
            print_info("Running time of the bucket protocol is", result.time.count());
            break;
        }
        case subset: {
            //    b_params.low_order_bits = 128;
            SubsetBatching subset_batch = SubsetBatching(_w_params, _b_params, xy, _trapdoor);
            result = subset_batch.batch(128);
            print_info("Running time of the subset protocol is", result.time.count());
            break;
        }
        case exponent: {
            Batching batch = Batching(_w_params, _b_params, xy, _trapdoor);
            result = batch.batch();
            print_info("Running time of the exponent protocol is", result.time.count());
            break;
        }
        case all_but_naive: {
            run_protocol(_w_params, _b_params, xy, _trapdoor, hybrid);
            run_protocol(_w_params, _b_params, xy, _trapdoor, bucket);
            run_protocol(_w_params, _b_params, xy, _trapdoor, subset);
            run_protocol(_w_params, _b_params, xy, _trapdoor, exponent);
            break;
        }
        case all: {
            run_protocol(_w_params, _b_params, xy, _trapdoor, all_but_naive);
            run_protocol(_w_params, _b_params, xy, _trapdoor, naive);
            break;
        }
    }
    return result;
}

void print_error(std::string error) {
    std::cout << "ERROR: " << error << std::endl;
}

std::map<std::string, protocoltype> str_protocol = {
        {"naive", naive},
        {"bucket", bucket},
        {"hybrid", hybrid},
        {"subset", subset},
        {"exponential", exponent},
        {"all", all},
        {"all_but_naive", all_but_naive}
};

runparams get_runparams(int cnt, char *args[], errortype &error) {
    runparams result = {};
    if (cnt <= 4) {
        error = {true, "not enough parameters"};
        return result;
    }

    std::vector<std::string> str_args = {};
    for (int i = 0; i < cnt; ++i) {
        std::string cur_arg(args[i]);
        str_args.push_back(cur_arg);
    }

    std::string protocol_arg = str_args[1];
    error.error = false;
    if (!str_protocol.count(protocol_arg)) {
        error = {true, "unknown protocol parameter (1) " + protocol_arg};
        return result;
    }
    result.protocol = str_protocol[protocol_arg];

    /// TODO: add errors when not numbers
    result.logt = std::stoi(str_args[2]);
    result.experiments = std::stoi(str_args[3]);
    result.instances_per_exp = std::stol(str_args[4]);

    // Argument parsing
    srand(time(NULL));

    mpz_helper helper = mpz_helper();
    bigint p = helper.generate_prime(1024);
    bigint q = helper.generate_prime(1024);
    bigint N = p * q;
    /// TODO: kopirovani? Pokud ano, memory leak;
    result.N = N;
    result.trapdoor = {p, q};

    print_info("Running the protocol " + protocol_arg + " with params...");
    return result;
}
