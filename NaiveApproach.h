//
// Created by Светлана Иванова on 24.09.2023.
//

#ifndef SRC_NAIVEAPPROACH_H
#define SRC_NAIVEAPPROACH_H


#include "mpz_helper.h"
#include "batching.h"

class NaiveApproach : public RandomExponents {
private:
    std::vector<bigint> l, pi;
public:
    NaiveApproach(WesolowskiParams _w_params, BatchingParams _b_params,
                  std::pair<std::vector<bigint>, std::vector<bigint>> xy,
                  std::pair<bigint, bigint> _trapdoor) : RandomExponents(_w_params, _b_params, xy, _trapdoor) {
        l.clear();
        pi.clear();
        l.resize(x.size());
        pi.resize(y.size());
    }
    BatchingResult batch() override;
};


#endif //SRC_NAIVEAPPROACH_H
