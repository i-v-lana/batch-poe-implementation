//
// Created by Светлана Иванова on 09.04.2024.
//
#include "batching.h"
#include "helper.h"

BatchingResult run_naive();
BatchingResult run_bucket();
BatchingResult run_hybrid();
BatchingResult run_subset();
BatchingResult run_exponent();

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
