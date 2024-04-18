//
// Created by Светлана Иванова on 21.11.2023.
//

#include "HybridBatching.h"

BatchingResult HybridBatching::batch(int m) {
    BatchingResult batch_result; batch_result.batch_y = {}; batch_result.batch_x = {}; batch_result.result = true;
    auto start_total = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < m; ++j) {
        /// should I resample the key for every run?
        std::vector<bool> take = SubsetBatching::get_take_inst(j);

        bigint batch_x = bigint(1);
        bigint batch_y = bigint(1);
        for (int i = 0; i < b_params.cnt; ++i) {
            if (take[i]) {
                batch_x = helper.mul_mod(batch_x, x[i], b_params.N);
                batch_y = helper.mul_mod(batch_y, y[i], b_params.N);
            }
        }
        batch_result.batch_x.push_back(batch_x);
        batch_result.batch_y.push_back(batch_y);
    }

    BatchingParams rothem_params = b_params;
    rothem_params.cnt = m;
    std::pair<std::vector<bigint>, std::vector<bigint> > batch_xy = {batch_result.batch_x, batch_result.batch_y};

    Batching rothem_batch = Batching(w_params, rothem_params, batch_xy, {p, q});

    auto end_total = std::chrono::high_resolution_clock::now();
    batch_result = rothem_batch.batch();
    batch_result.time += end_total - start_total;
    return batch_result;
}