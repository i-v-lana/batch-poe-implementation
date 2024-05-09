//
// Created by Светлана Иванова on 15.12.2023.
//

#ifndef SRC_BUCKETBATCHING_H
#define SRC_BUCKETBATCHING_H

#include "wesolowski.h"
#include "mpz_helper.h"
#include <fstream>
#include "PRF_crypto.h"
#include "batching.h"


class BucketBatching : public RandomExponents {
private:
    std::vector<std::vector<int> > buckets;
    int repeat = 0;
    PRF_crypto bucket_prf = PRF_crypto(bigint(), iv, 128);
    int bucket_bit = 0;
    int bucket_num = 0;
    void init_bucket(int _bucket_bit);
public:
    BucketBatching(WesolowskiParams _w_params, BatchingParams _b_params,
            std::pair<std::vector<bigint>, std::vector<bigint>> xy,
    std::pair<bigint, bigint> _trapdoor) : RandomExponents(_w_params, _b_params, xy, _trapdoor) {}
    BatchingResult batch(int _bucket_bit=5);
};


#endif //SRC_BUCKETBATCHING_H
