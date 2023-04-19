//
// Created by Светлана Иванова on 19.04.2023.
//

#include "batching.h"

void Batching::generate(int _cnt) {
    cnt = _cnt;
    for (int i = 0; i < cnt; ++i) {
        mpz_init(x_instances[i]);
        vdf.generate(x_instances[i]);
    }
}

Batching::Batching(int _lambda, int _k, int _cnt) {
    vdf = Wesolowski();
    vdf.setup(_lambda, _k);
    generate(_cnt);
}

void Batching::print(std::ofstream& file) {
    for (int i = 0; i < cnt; ++i)
        file << x_instances[i] << std::endl;
}
