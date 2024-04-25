//
// Created by Светлана Иванова on 09.04.2024.
//

#ifndef SRC_HELPER_H
#define SRC_HELPER_H
#include "batching.h"
#include <map>

struct errortype {
    bool error = false;
    std::string error_msg = "";
};
void print_error(std::string error);
void print_info(std::string info);

enum protocoltype {naive, bucket, hybrid, subset, exponent, all, all_but_naive};
extern std::map<std::string, protocoltype> str_protocol;

struct runparams {
    protocoltype protocol;
    int logt;
    int experiments;
    long instances_per_exp;
    bigint N;
    std::pair<bigint, bigint> trapdoor;
    std::string output_file="";
};
runparams get_runparams(int cnt, char *args[], errortype &error);


BatchingResult run_protocol(WesolowskiParams _w_params, BatchingParams _b_params,
                            std::pair<std::vector<bigint>, std::vector<bigint>> xy, std::pair<bigint, bigint> _trapdoor,
                            protocoltype _protocol, std::string _output_path="");


#endif //SRC_HELPER_H
