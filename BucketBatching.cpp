//
// Created by Светлана Иванова on 15.12.2023.
//

#include "BucketBatching.h"

BatchingResult BucketBatching::batch(int _bucket_bit) {
    auto default_value = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = (default_value - default_value);

    init_bucket(_bucket_bit);
    BatchingResult batch_result; batch_result.batch_y.resize(repeat, bigint(1)); batch_result.batch_x.resize(repeat, bigint(1)); batch_result.result = true;
    std::cout << "Running batch() for Bucket protocol with bucket_bit = " << _bucket_bit << "; and repeat = " << repeat << std::endl;
    w_params.print();
    b_params.print();

    for (int j = 0; j < repeat; ++j) {
        auto start = std::chrono::high_resolution_clock::now();
        /// should I resample the key for every run?
        /// generating buckets
        buckets.clear();
        buckets.resize(bucket_num, {});
        for (int i = 0; i < b_params.cnt; ++i) {
            bigint cur_i = bigint(j) + bigint(i) * bigint(b_params.cnt);
            buckets[bucket_prf.evaluate(cur_i).get_num()].push_back(i);
        }

        /// Preparing instances from buckets
        std::vector<bigint> buckets_x(bucket_num, bigint(1)), buckets_y(bucket_num, bigint(1));
        for (int i = 0; i < bucket_num; ++i) {
            for (int ind: buckets[i]) {
                buckets_x[i] = helper.mul_mod(buckets_x[i], x[ind], b_params.N);
                buckets_y[i] = helper.mul_mod(buckets_y[i], y[ind], b_params.N);
            }
        }
        /// Batching instances from buckets into 1 with Random Exponent
        /// init
        BatchingParams rothem_params = b_params;
        rothem_params.cnt = bucket_num;
        rothem_params.low_order_bits = bucket_bit;
        auto end = std::chrono::high_resolution_clock::now();
        std::pair<std::vector<bigint>, std::vector<bigint> > buckets_xy = {buckets_x, buckets_y};
        Batching rothem_batch = Batching(w_params, rothem_params, buckets_xy, {p, q});
        /// run
        auto rothem_batch_result = rothem_batch.combine();
        total_time += (end - start) + rothem_batch_result.time;


        start = std::chrono::high_resolution_clock::now();
        batch_result.batch_x[j] = rothem_batch_result.batch_x[0];
        batch_result.batch_y[j] = rothem_batch_result.batch_y[0];
        end = std::chrono::high_resolution_clock::now();
        total_time += (end - start);
    }

    /// Batching instances from repeated run of the protocol
    /// init
    BatchingParams rothem_params = b_params;
    rothem_params.cnt = repeat;
    std::pair<std::vector<bigint>, std::vector<bigint> > batch_xy = {batch_result.batch_x, batch_result.batch_y};
    Batching rothem_batch = Batching(w_params, rothem_params, batch_xy, {p, q});
    /// run
    batch_result = rothem_batch.batch();
    batch_result.time += total_time;
    return batch_result;
}

void BucketBatching::init_bucket(int _bucket_bit) {
    bucket_bit = _bucket_bit;
    bucket_num = pow(2, bucket_bit);
    repeat = ceil((double)w_params.k / ((double)bucket_bit - 2.0));
    std::string _iv = "IVinput";
    bucket_prf = PRF_crypto(helper.get_random(b_params.lambda_prf), _iv, bucket_bit);
}
