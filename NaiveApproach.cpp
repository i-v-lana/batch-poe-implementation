//
// Created by Светлана Иванова on 24.09.2023.
//

#include "NaiveApproach.h"
#include "wesolowski.h"

BatchingResult NaiveApproach::batch() {
    for (int i = 0; i < x.size(); ++i) {
        batch_prover_part(&l[i], &pi[i], x[i]);
    }
    BatchingResult result = BatchingResult();
    result.result = true;
    for (int i = 0; i < x.size(); ++i) {
        auto cur_result = batch_verifier_part(x[i], y[i], l[i], pi[i]);
        result.result = result.result && cur_result.first;
        result.time += cur_result.second;
    }
    result.batch_x = x;
    result.batch_y = y;
    return result;
}
