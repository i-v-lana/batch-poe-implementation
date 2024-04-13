#ifndef WESOLOWSKI_H
#define WESOLOWSKI_H

#include <gmp.h>
#include <vector>
#include <chrono>

#include "proof.h"

class Wesolowski {
public:
    Wesolowski();
    ~Wesolowski() {
        mpz_clear(N);
        free(rstate);
    }

    void setup(int k, const mpz_t& _N, long long _t);
    Proof prover_trapdoor(mpz_t l, mpz_t pi, const mpz_t x, const mpz_t phi_N);
    Proof prover(mpz_t l, mpz_t pi, const mpz_t x);
    bool verifier(mpz_t x, mpz_t y_check, mpz_t l, mpz_t pi);

    std::chrono::duration<double> verif_time;

private:
    mpz_t N;
    int k;
    long long t;
    gmp_randstate_t rstate;
    void sample_prime(mpz_t l, const mpz_t x);
};

#endif