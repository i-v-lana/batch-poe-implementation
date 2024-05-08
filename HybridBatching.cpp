//
// Created by Светлана Иванова on 21.11.2023.
//

#include "HybridBatching.h"

BatchingResult HybridBatching::batch(int m) {
    BatchingResult batch_result; batch_result.result = true;
    batch_result.batch_y.resize(m, bigint(1)); batch_result.batch_x.resize(m, bigint(1));

    auto default_start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = default_start - default_start;
    for (int j = 0; j < m; ++j) {
        std::vector<int> taken = {};
        SubsetBatching::get_take_inst(j, taken);

        auto start = std::chrono::high_resolution_clock::now();

        for (int ind : taken) {
            helper.mul_mod(batch_result.batch_x[j], batch_result.batch_x[j], x[ind], b_params.N);
            helper.mul_mod(batch_result.batch_y[j], batch_result.batch_y[j], y[ind], b_params.N);
        }

        auto finish = std::chrono::high_resolution_clock::now();

        total_time += (finish - start);

    }

    BatchingParams rothem_params = b_params;
    rothem_params.cnt = m;
    std::pair<std::vector<bigint>, std::vector<bigint> > batch_xy = {batch_result.batch_x, batch_result.batch_y};

    Batching rothem_batch = Batching(w_params, rothem_params, batch_xy, {p, q});

    batch_result = rothem_batch.batch();
    batch_result.time += total_time;
    return batch_result;
}