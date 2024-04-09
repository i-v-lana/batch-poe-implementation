//
// Created by Светлана Иванова on 09.04.2024.
//
#include "batching.h"
#include "helper.h"
#include "HybridBatching.h"
#include "BucketBatching.h"
#include "SubsetBatching.h"

BatchingResult run_protocol(WesolowskiParams _w_params, BatchingParams _b_params,
                          std::pair<std::vector<bigint>, std::vector<bigint>> xy,
                          std::pair<bigint, bigint> _trapdoor, protocoltype _protocol) {
    BatchingResult result;
    switch(_protocol) {
        /// TODO: why 128? wrap it to the parameter? why 11?
        case hybrid:
            _b_params.low_order_bits = 128;
            HybridBatching hybrid_batch = HybridBatching(_w_params, _b_params, xy, _trapdoor);
            result = hybrid_batch.batch(128);
            break;
        case naive:
            break;
        case bucket:
            BucketBatching bucket_batch = BucketBatching(_w_params, _b_params, xy, _trapdoor);
            result = bucket_batch.batch(11);
            break;
        case subset:
            SubsetBatching subset_batch = SubsetBatching(_w_params, _b_params, xy, _trapdoor);
            result = subset_batch.batch(128);
            break;
        case exponent:
            Batching batch = Batching(_w_params, _b_params, xy, _trapdoor);
            result = batch.batch();
            break;
    }
}

void print_error(std::string& error) {
    std::cout << "ERROR: " << error << std::endl;
}

void print_info(std::string info) {
    std::cout << "INFO: " << info << std::endl;
}

std::map<std::string, protocoltype> str_protocol = {
        {"naive", naive},
        {"bucket", bucket},
        {"hybrid", hybrid},
        {"subset", subset},
        {"exponent", exponent}
};

runparams get_runparams(int cnt, char *args[], errortype &error) {
    runparams result = {};
    if (cnt <= 1) {
        error = {true, "not enough parameters"};
        return result;
    }
    std::string protocol_arg(args[1]);
    error.error = false;
    if (!str_protocol.count(protocol_arg)) {
        error = {true, "unknown protocol parameter (1) " + protocol_arg};
        return result;
    }
    result.protocol = str_protocol[protocol_arg];

    print_info("Running the protocol " + protocol_arg + " with params...");
    return result;
}
