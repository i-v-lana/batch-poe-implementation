//
// Created by Светлана Иванова on 11.04.2024.
//

#ifndef SRC_EXPERIMENTS_H
#define SRC_EXPERIMENTS_H
#include "mpz_helper.h"


class Experiments {
public:
    Experiments() = default;
    /// TODO:
    /// Experiments(source, connection_key);
    std::vector<std::pair<std::vector<bigint>, std::vector<bigint> > > get(long instances_per_exp, int exp);
};


#endif //SRC_EXPERIMENTS_H
