//
// Created by Светлана Иванова on 21.11.2023.
//

#include "HybridBatching.h"

BatchingResult HybridBatching::batch(int m) {
    BatchingResult batch_result; batch_result.batch_y.resize(m, bigint(1)); batch_result.batch_x.resize(m, bigint(1)); batch_result.result = true;
    std::cout << "Running batch() for the hybrid protocol with m = " << m << std::endl;
    w_params.print();
    b_params.print();
    auto start_total = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < m; ++j) {
        /// should I resample the key for every run?
        std::vector<bool> take = {};
        SubsetBatching::get_take_inst(j, take);

        for (int i = 0; i < b_params.cnt; ++i) {
            if (take[i]) {
                batch_result.batch_x[j] = helper.mul_mod(batch_result.batch_x[j], x[i], b_params.N);
                batch_result.batch_y[j] = helper.mul_mod(batch_result.batch_y[j], y[i], b_params.N);
            }
        }
    }

    BatchingParams rothem_params = b_params;
    rothem_params.cnt = m;
    auto end_total = std::chrono::high_resolution_clock::now();
    std::pair<std::vector<bigint>, std::vector<bigint> > batch_xy = {batch_result.batch_x, batch_result.batch_y};

    Batching rothem_batch = Batching(w_params, rothem_params, batch_xy, {p, q});

    batch_result = rothem_batch.batch();
    batch_result.time += (end_total - start_total);
    return batch_result;
}