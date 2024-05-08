//
// Created by Светлана Иванова on 06.11.2023.
//

#include "SubsetBatching.h"

BatchingResult SubsetBatching::batch(int m) {
    auto default_value = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = {};
    total_time = default_value - default_value;
    BatchingResult batch_result; batch_result.batch_y = {}; batch_result.batch_x = {}; batch_result.result = true;
    for (int j = 0; j < m; ++j) {
        auto start_total = std::chrono::high_resolution_clock::now();

        std::vector<int> taken = {};

        get_take_inst(j, taken);
        bigint batch_x = bigint(1);
        bigint batch_y = bigint(1);
        for (int ind : taken) {
            helper.mul_mod(batch_x, batch_x, x[ind], b_params.N);
            helper.mul_mod(batch_y, batch_y, y[ind], b_params.N);
        }

        bigint _l = bigint();
        bigint _pi = bigint();
        auto end_total = std::chrono::high_resolution_clock::now();
        batch_prover_part(&_l, &_pi, batch_x);
        std::pair<bool, std::chrono::duration<double>> result = batch_verifier_part(batch_x, batch_y, _l, _pi);

        total_time += end_total - start_total + result.second;
        batch_result.result = batch_result.result && result.first;
        batch_result.batch_x.push_back(batch_x);
        batch_result.batch_y.push_back(batch_y);
    }
    batch_result.time = total_time;
    return batch_result;
}

void SubsetBatching::get_take_inst(int run_num, std::vector<int> &subset) {
    /// TODO: should I resample the key for every run?
    int repeat_times = ceil((double)b_params.cnt / (double)prf.length());
    int cur_ind = 0;
    for (int i = 0; i < repeat_times; ++i) {
        bigint prf_arg = bigint(i) + bigint(run_num) * bigint(repeat_times);

        std::vector<bool> cur_take = prf.evaluate(prf_arg).get_bool_bits(prf.length());
        for (bool take : cur_take) {
            if (cur_ind >= b_params.cnt) {
                return;
            }

            if (take) {
                subset.push_back(cur_ind);
            }
            ++cur_ind;
        }
    }
}

