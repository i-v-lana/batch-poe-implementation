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
    bigint(long x) {
        mpz_init(num);
        mpz_set_ui(num, x);
    }
    bigint() {
        mpz_init(num);
    }
    void from_bytes(std::string bytes, int bits_cnt) {
        mpz_init(num);
        for (char byte : bytes) {
            std::bitset<8> bits(byte);

            for (int i = 7; i >= 0; --i) {
                mpz_mul_2exp(num, num, 1);
                mpz_add_ui(num, num, bits[i]);
            }

        }
        mpz_set(num, first_n_bits(bits_cnt).num);
    }
    int bits() {
        return mpz_sizeinbase(num, 2);
    }
    unsigned long int get_num() {
        return mpz_get_ui(num);
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
    std::string bytes_string() {
        mpz_t cur;
        mpz_init(cur);
        std::string bytes = "";
        int bytes_len = (this->bits() + 7) / 8;
        for (size_t i = 0; i < bytes_len; i++) {
            mpz_set(cur, num);
            mpz_div_2exp(cur, cur, 8 * i);
            char byte = mpz_get_ui(cur) & 0xFF;
            std::cout << byte << std::endl;
            bytes.push_back(byte);
        }
        std::cout << bytes << std::endl;
        return bytes;
    }
    bool operator == (const mpz_t& other) const {
        return !(mpz_cmp(this->num, other));
    }
    bool operator == (const bigint& other) const {
        return !(mpz_cmp(this->num, other.num));
    }
    bigint operator - (const unsigned long& x) const {
        bigint ans = bigint();
        mpz_sub_ui(ans.num, num, x);
        return ans;
    }
    bigint operator * (const bigint& x) const {
        bigint ans = bigint();
        mpz_mul(ans.num, num, x.num);
        return ans;
    }
    bool operator != (int& other) const {
        return mpz_cmp_si(this->num, other);
    }
};

class mpz_helper {
public:
    bigint get_random(int bit_cnt);
    bigint get_random_mod(bigint mod);
    bigint generate_prime(int bit_cnt);
    bigint gcd(bigint &a, bigint &b);
    bigint pow(bigint x, bigint p, bigint mod);
    bigint mul_mod(bigint a, bigint b, bigint mod);
    mpz_helper();
private:
    gmp_randstate_t state;
};

std::ostream& operator << (std::ostream& os, const bigint& obj);


#endif //SRC_MPZ_HELPER_H
