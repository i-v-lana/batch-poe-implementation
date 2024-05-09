//
// Created by Светлана Иванова on 11.04.2024.
//

#ifndef SRC_EXPERIMENTS_H
#define SRC_EXPERIMENTS_H
#include "mpz_helper.h"
#include <random>


class Experiments {
private:
    std::vector<bigint> x, y;
    std::uniform_int_distribution<int> index;
    std::random_device rd;
    std::default_random_engine generator;

    bigint N, p, q;
    long long t;
    mpz_helper helper;
    bool trapdoor_flag = false;

    bool check_trapdoor(bigint& _x);
    void set_trapdoor(std::pair<bigint, bigint> _trapdoor);
    bigint trapdoor(bigint& _x);
public:
    Experiments() = default;
    Experiments(int sample_size, bigint& mod_N, std::pair<bigint, bigint> _trapdoor, long long _t);
    std::vector<std::pair<std::vector<bigint>, std::vector<bigint> > > get(long instances_per_exp, int exp);
};


#endif //SRC_EXPERIMENTS_H
