//
// Created by Светлана Иванова on 19.04.2023.
//

#include "batching.h"

void Batching::generate(int _cnt) {
    cnt = _cnt;
    for (int i = 0; i < cnt; ++i) {
        mpz_init(x_instances[i]);
        vdf[i].generate(x_instances[i]);
    }
}

Batching::Batching(int _lambda, int _k, int _t, int _cnt) {
    for (int i = 0; i < cnt; ++i) {
        vdf[i] = Wesolowski();
        vdf[i].setup(_lambda, _k);
    }
    t = _t;
    generate(_cnt);
}

void Batching::evaluate() {
    for (int i = 0; i < cnt; ++i) {
        mpz_t l, pi;
        mpz_init(l);
        mpz_init(pi);
        vdf[i].evaluate(l, pi, x_instances[i], pow(2, t));
    }
}

void Batching::print(std::ofstream& file) {
    for (int i = 0; i < cnt; ++i)
        file << x_instances[i] << std::endl;
}
