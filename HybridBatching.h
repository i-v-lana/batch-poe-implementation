//
// Created by Светлана Иванова on 21.11.2023.
//

#ifndef SRC_HYBRIDBATCHING_H
#define SRC_HYBRIDBATCHING_H

#include <vector>
#include <gmp.h>
#include "wesolowski.h"
#include "mpz_helper.h"
#include <fstream>
#include "PRF_crypto.h"
#include "batching.h"

class HybridBatching : public Batching {
public:
    HybridBatching(WesolowskiParams _w_params, BatchingParams _b_params,
                   std::pair<std::vector<bigint>, std::vector<bigint>> xy,
                   std::pair<bigint, bigint> _trapdoor) : Batching(_w_params, _b_params, xy, _trapdoor) {}
    BatchingResult batch(int m=100);
};

#endif //SRC_HYBRIDBATCHING_H
