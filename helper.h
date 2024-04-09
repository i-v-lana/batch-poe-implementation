//
// Created by Светлана Иванова on 09.04.2024.
//

#ifndef SRC_HELPER_H
#define SRC_HELPER_H

struct errortype {
    bool error = false;
    std::string error_msg = "";
};
void print_error(std::string& error);
void print_info(std::string info);

enum protocoltype {naive, bucket, hybrid, subset, exponent};
extern std::map<std::string, protocoltype> str_protocol;

struct runparams {
    protocoltype protocol;
};
runparams get_runparams(int cnt, char *args[], errortype &error);


BatchingResult run_naive();
BatchingResult run_bucket();
BatchingResult run_hybrid();
BatchingResult run_subset();
BatchingResult run_exponent();


#endif //SRC_HELPER_H
