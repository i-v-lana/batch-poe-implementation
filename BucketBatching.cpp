//
// Created by Светлана Иванова on 15.12.2023.
//

#include "BucketBatching.h"

BatchingResult BucketBatching::batch(int _bucket_bit) {
    /// TODO: batch_result by nemel podporovat vektory, je to nekonzistentni
    BatchingResult batch_result; batch_result.batch_y = {}; batch_result.batch_x = {}; batch_result.result = true;
    auto default_value = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = (default_value - default_value);

    init_bucket(_bucket_bit);

    for (int j = 0; j < repeat; ++j) {
        auto start = std::chrono::high_resolution_clock::now();
        /// should I resample the key for every run?
        /// generating buckets
        buckets.clear();
        buckets.resize(bucket_num, {});
        for (int i = 0; i < b_params.cnt; ++i) {
            bigint cur_i = bigint(j) + bigint(i);
            /// TODO: FIX prf evaluation
            buckets[bucket_prf.evaluate(cur_i).get_num()].push_back(i);
        }
        /// Preparing instances from buckets
        std::vector<bigint> buckets_x(0), buckets_y(0);
        for (int i = 0; i < bucket_num; ++i) {
            bigint cur_x = bigint(1);
            bigint cur_y = bigint(1);
            for (int ind: buckets[i]) {
                cur_x = helper.mul_mod(cur_x, x[ind], b_params.N);
                cur_y = helper.mul_mod(cur_y, y[ind], b_params.N);
            }
            buckets_x.push_back(cur_x);
            buckets_y.push_back(cur_y);
        }
        /// Batching instances from buckets into 1 with Random Exponent
        /// init
        /// TODO: no wesolowski proofs
        BatchingParams rothem_params = b_params;
        /// TODO: fix, low_order bits to k.
        rothem_params.cnt = bucket_num;
        Batching rothem_batch = Batching(w_params, rothem_params, {buckets_x, buckets_y}, {p, q});
        auto end = std::chrono::high_resolution_clock::now();
        /// run
        auto rothem_batch_result = rothem_batch.batch();
        total_time += (end - start) + rothem_batch_result.time;


        start = std::chrono::high_resolution_clock::now();
        batch_result.batch_x.push_back(rothem_batch_result.batch_x[0]);
        batch_result.batch_y.push_back(rothem_batch_result.batch_y[0]);
        end = std::chrono::high_resolution_clock::now();
        total_time += (end - start);
    }

    /// Batching instances from repeated run of the protocol
    /// init
    auto start = std::chrono::high_resolution_clock::now();
    BatchingParams rothem_params = b_params;
    rothem_params.cnt = repeat;
    Batching rothem_batch = Batching(w_params, rothem_params, {batch_result.batch_x, batch_result.batch_y}, {p, q});
    auto end = std::chrono::high_resolution_clock::now();
    /// run
    batch_result = rothem_batch.batch();
    batch_result.time += total_time + (end - start);
    std::cout << "BATCHING: Total time of the bucket batching protocol: " << batch_result.time.count() << std::endl;
    return batch_result;
}

void BucketBatching::init_bucket(int _bucket_bit) {
    bucket_bit = _bucket_bit;
    bucket_num = pow(2, bucket_bit);
    repeat = ceil((double)w_params.k / ((double)bucket_bit - 2.0));
    std::string _iv = "IVinput";
    bucket_prf = PRF_crypto(helper.get_random(b_params.lambda_prf), _iv, bucket_bit);
}
