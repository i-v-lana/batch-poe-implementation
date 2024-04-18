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
#include <vector>


struct bigint {
    mpz_t num;
    bigint(const mpz_t& x) {
        mpz_init(num);
        mpz_set(num, x);
    }
    bigint(const bigint& x) {
        mpz_init(num);
        mpz_set(num, x.num);
    }
    bigint(const char *s) {
        mpz_init(num);
        mpz_set_str(num, s, 10);
    }
    bigint(std::string& s) {
        std::string s1 = s;
        mpz_init(num);
        mpz_set_str(num, s1.c_str(), 10);
    }
    bigint(int x) {
        mpz_init(num);
        mpz_set_ui(num, x);
    }
    bigint(long x) {
        mpz_init(num);
        mpz_set_ui(num, x);
    }
    bigint(long long x) {
        mpz_init(num);
        mpz_set_ui(num, x);
    }
    bigint(const std::string& bytes, int bits_cnt=-1) {
        mpz_init(num);
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
    bigint() {
        mpz_init(num);
    }
    bigint& operator=(const bigint& other) {
        if (this != &other) {
            mpz_init(num);
            mpz_set(num, other.num);
        }
        return *this;
    }
    ~bigint() {
        mpz_clear(num);
    }
    int bits() {
        return mpz_sizeinbase(num, 2);
    }
    unsigned long int get_num() {
        return mpz_get_ui(num);
    }
    bigint last_n_bits(int &n) {
        bigint ans = bigint();
        int exp = std::max(n, 0);
        mpz_tdiv_r_2exp(ans.num, num, exp);
        return ans;
    }
    std::string bytes_string(int bytes_cnt) {
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
    std::vector<bool> get_bool_bits(int bits_cnt) {
        bigint temp = bigint(num);
        std::vector<bool> result;

        while (mpz_sgn(temp.num) > 0) {
            result.push_back(mpz_odd_p(temp.num) != 0);
            mpz_fdiv_q_2exp(temp.num, temp.num, 1);
        }

        int bits_len = this->bits();
        while (bits_len < bits_cnt) {
            result.push_back(false);
            ++bits_len;
        }

        // Reverse the vector to get the correct bit order
        std::reverse(result.begin(), result.end());
        return result;
    };
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
    bigint operator + (const bigint& x) const {
        bigint ans = bigint();
        mpz_add(ans.num, num, x.num);
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
    bigint get_random(int &bit_cnt);
    bigint get_random_mod(bigint &mod);
    bigint generate_prime(int bit_cnt);
    bigint gcd(bigint &a, bigint &b);
    bigint pow(bigint &x, bigint &p, bigint &mod);
    bigint mul_mod(bigint &a, bigint &b, bigint &mod);
    mpz_helper();
private:
    gmp_randstate_t state;
};

std::ostream& operator << (std::ostream& os, const bigint& obj);


#endif //SRC_MPZ_HELPER_H
