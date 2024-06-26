//
// Created by Светлана Иванова on 02.08.2023.
//

#include "mpz_helper.h"

bigint mpz_helper::get_random(int &bit_cnt) {
    bigint cur_rand = bigint();
    mpz_init(cur_rand.num);
    mpz_urandomb(cur_rand.num, state, bit_cnt);
    return cur_rand;
}

bigint mpz_helper::generate_prime(int bit_cnt) {
    bigint cur_rand = get_random(bit_cnt);
    mpz_nextprime(cur_rand.num, cur_rand.num);
    return cur_rand;
}

bigint mpz_helper::pow(bigint &x, bigint &p, bigint &mod) {
    bigint ans = bigint();
    mpz_powm(ans.num, x.num, p.num, mod.num);
    return ans;
}

mpz_helper::mpz_helper() {
    gmp_randinit_mt(state);
}

/// res = (a * b) % mod
void mpz_helper::mul_mod(bigint &res, bigint &a, bigint &b, bigint &mod) {
    mpz_mul(res.num, a.num, b.num);
    mpz_mod(res.num, a.num, mod.num);
}

bigint mpz_helper::get_random_mod(bigint &mod) {
    bigint ans = bigint();
    mpz_urandomm(ans.num, state, mod.num);
    return ans;
}

bigint mpz_helper::gcd(bigint& a, bigint& b) {
    bigint ans = bigint();
    mpz_gcd(ans.num, a.num, b.num);
    return ans;
}

std::ostream& operator << (std::ostream& os, const bigint& obj) {
    char* str = mpz_get_str(NULL, 10, obj.num);
    os << str;
    free(str);
    return os;
}
