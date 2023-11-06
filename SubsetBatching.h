//
// Created by Светлана Иванова on 06.11.2023.
//

#ifndef SRC_SUBSETBATCHING_H
#define SRC_SUBSETBATCHING_H
#include <vector>
#include <gmp.h>
#include "wesolowski.h"
#include "mpz_helper.h"
#include <fstream>
#include "PRF_crypto.h"
#include "batching.h"

class SubsetBatching : public batching {
protected:
    void batch_prover_part(bigint* _l, bigint* _pi, bigint batch_x);
    std::pair<bool, std::chrono::duration<double>> batch_verifier_part(bigint batch_x, bigint batch_y, bigint _l, bigint _pi);
public:
    SubsetBatching(WesolowskiParams _w_params, BatchingParams _b_params, std::pair<bigint, bigint> _trapdoor);
    SubsetBatching(WesolowskiParams _w_params, BatchingParams _b_params, std::vector<std::pair<bigint, bigint> > xy);
    void print(std::ofstream& file);
    void print_cout();
    std::pair<std::vector<bigint>, std::vector<bigint> > get_instances();
    BatchingResult batch();
};

#endif //SRC_SUBSETBATCHING_H
