//
// Created by Светлана Иванова on 06.11.2023.
//

#include "SubsetBatching.h"

BatchingResult SubsetBatching::batch(int m) {
    std::chrono::duration<double> multi_time, total_time;
    BatchingResult batch_result; batch_result.batch_y = {}; batch_result.batch_x = {}; batch_result.result = true;
    for (int j = 0; j < m; ++j) {
        auto start_total = std::chrono::high_resolution_clock::now();
        /// should I resample the key for every run?
        alpha.clear();
        alpha.resize(b_params.cnt);
        for (int i = 0; i < ceil((double)b_params.cnt / (double)prf.length()); ++i) {
            bigint cur_i = bigint(j) + bigint(i);
            /// TODO: FIX
            alpha[i] = prf.evaluate(cur_i);
        }
        bigint batch_x = bigint(1);
        bigint batch_y = bigint(1);
        int alpha_ind = -1;
        std::vector<bool> alpha_bits;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < b_params.cnt; ++i) {
            if (i % prf.length() == 0) {
                ++alpha_ind;
                alpha_bits = alpha[alpha_ind].get_bool_bits(prf.length());
            }
            bool take = alpha_bits[i % prf.length()];
            if (take) {
                batch_x = helper.mul_mod(batch_x, x[i], b_params.N);
                batch_y = helper.mul_mod(batch_y, y[i], b_params.N);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        bigint _l = bigint();
        bigint _pi = bigint();
        auto end_total = std::chrono::high_resolution_clock::now();
        batch_prover_part(&_l, &_pi, batch_x);
        std::pair<bool, std::chrono::duration<double>> result = batch_verifier_part(batch_x, batch_y, _l, _pi);

        multi_time = multi_time + end - start; //+ result.second;
        total_time += end_total - start_total + result.second;
        batch_result.result = batch_result.result && result.first;
        batch_result.batch_x.push_back(batch_x);
        batch_result.batch_y.push_back(batch_y);
    }
    std::cout << "BATCHING: Total time of the random subset batching protocol: " << total_time.count() << std::endl;
    return batch_result;
}

