//
// Created by Светлана Иванова on 02.08.2023.
//

#ifndef SRC_MPZ_HELPER_H
#define SRC_MPZ_HELPER_H


#include <gmp.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <crypto++/config.h>


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
    void from_bytes(const std::string& bytes, int bits_cnt=-1) {
        mpz_set_ui(num, 0);
        for (char byte : bytes) {
            std::bitset<8> bits(byte);
            for (int i = 7; i >= 0; --i) {
                mpz_mul_2exp(num, num, 1);
                mpz_add_ui(num, num, bits[i]);
            }
        }
        if (bits_cnt != -1)
            mpz_set(num, last_n_bits(bits_cnt).num);
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
        int exp = std::max(len - n, 0);
        mpz_tdiv_q_2exp(ans.num, num, exp);
        return ans;
    }
    bigint last_n_bits(int n) {
        bigint ans = bigint();
        int len = bits();
        int exp = std::max(n, 0);
        mpz_tdiv_r_2exp(ans.num, num, exp);
        return ans;
    }
    std::string bytes_string(int bytes_cnt) {
        mpz_t cur;
        mpz_init(cur);
        std::string bytes = "";
        int bytes_len = (this->bits() + 7) / 8;
        for (size_t i = 0; i < bytes_cnt - bytes_len; ++i) {
            bytes.push_back(0x00);
        }
        size_t size;
        unsigned char* byteArray = (unsigned char*)mpz_export(NULL, &size, 1, sizeof(unsigned char), 0, 0, num);
        for (int i = 0; i < size; ++i) {
            bytes.push_back(0xFF & byteArray[i]);
        }
        free(byteArray);
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
