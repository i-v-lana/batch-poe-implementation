//
// Created by Светлана Иванова on 15.12.2023.
//

#ifndef SRC_GENINSTANCES_H
#define SRC_GENINSTANCES_H

#include "mpz_helper.h"


class GenInstances {
private:
    bigint N, p, q;
    long t;
    mpz_helper helper;

    void set_trapdoor(bigint& _p, bigint& _q);
    bigint trapdoor(bigint& _x);
public:
    GenInstances() = default;
    GenInstances(bigint _N, std::pair<bigint, bigint> _trapdoor, long _t);
    void gen(long long cnt, std::string file_name);
};


#endif //SRC_GENINSTANCES_H
