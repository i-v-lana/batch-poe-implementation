//
// Created by Светлана Иванова on 19.04.2023.
//

#ifndef SRC_BATCHING_H
#define SRC_BATCHING_H


#include <vector>
#include <gmp.h>
#include "wesolowski.h"
#include <fstream>

class Batching {
private:
    mpz_t x_instances[1000];
    mpz_t y[1000];
    int cnt;
    int t;
    Wesolowski vdf[1000];
    void generate(int _cnt);
public:
    Batching(int _lambda, int _k, int _t, int _cnt);
    void print(std::ofstream& file);
    void evaluate();
};


#endif //SRC_BATCHING_H
