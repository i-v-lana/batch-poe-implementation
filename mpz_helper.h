//
// Created by Светлана Иванова on 02.08.2023.
//

#ifndef SRC_MPZ_HELPER_H
#define SRC_MPZ_HELPER_H


#include <gmp.h>
#include <cmath>
#include <iostream>

struct bigint {
    mpz_t num;
    bigint(mpz_t x) {
        mpz_init(num);
        mpz_set(num, x);
    }
    bigint(int x) {
        mpz_init(num);
        mpz_set_ui(num, x);
    }
    bigint() {
        mpz_init(num);
    }
    int bits() {
        return mpz_sizeinbase(num, 2);
    }
    bigint last_half(int len) {
        bigint ans = bigint();
        int half = len / 2;
        mpz_tdiv_r_2exp(ans.num, num, half);
        return ans;
    }
    bigint first_half(int len) {
        bigint ans = bigint();
        int half = len / 2;
        mpz_tdiv_q_2exp(ans.num, num, half);
        return ans;
    }
    bigint first_n_bits(int n) {
        bigint ans = bigint();
        int len = bits();
        mpz_tdiv_q_2exp(ans.num, num, len - n);
        return ans;
    }
};

class mpz_helper {
public:
    bigint get_random(int bit_cnt);
    bigint get_random_mod(bigint mod);
    bigint generate_prime(int bit_cnt);
    bigint pow(bigint x, bigint p, bigint mod);
    bigint mul_mod(bigint a, bigint b, bigint mod);
    mpz_helper();
private:
    gmp_randstate_t state;
};


#endif //SRC_MPZ_HELPER_H
