#include <chrono>
#include "wesolowski.h"
#include <gmpxx.h>
#include "mpz_helper.h"

void Wesolowski::sample_prime(mpz_t l, const mpz_t x) {
    mpz_t num, gcd, one;
    mpz_init(num);
    mpz_init(gcd);
    mpz_init(one);

    mpz_set_ui(one, 1);
    /// choose random number from the set 1...2^(2k)
    mpz_urandomb(num, rstate, 2*k);

    mpz_nextprime(l, num);
    mpz_gcd(gcd, l, x);
    /// l and x should be coprime - we rechoose num until l is coprime with x.
    while (mpz_cmp(gcd, one)) {
        mpz_urandomb(num, rstate, 2*k);
        mpz_nextprime(l, num);
        mpz_gcd(gcd, l, x);
    }

    mpz_clear(num);
    mpz_clear(gcd);
    mpz_clear(one);
}

Wesolowski::Wesolowski() {}

void Wesolowski::setup(int _k, const mpz_t& _N, long long _t) {
    k = _k;
    gmp_randinit_mt(rstate);
    mpz_init(N);
    mpz_set(N, _N);
    t = _t;
}

Proof Wesolowski::prover_trapdoor(mpz_t l, mpz_t pi, const mpz_t x, const mpz_t phi_N) {
    mpz_t exponent;
    mpz_init(exponent);
    mpz_ui_pow_ui(exponent, 2, t);

    sample_prime(l, x);

    mpz_t q;
    mpz_init(q);
    mpz_fdiv_q(q, exponent, l);

    /// Euler's theorem: x ^ q = x ^ (q mod phi(N))
    mpz_fdiv_r(q, q, phi_N);
    mpz_powm(pi, x, q, N);

    mpz_clear(exponent);
    mpz_clear(q);
    Proof proof_sent = Proof();
    return proof_sent;
}

Proof Wesolowski::prover(mpz_t l, mpz_t pi, const mpz_t x) {
    mpz_t exponent;
    mpz_init(exponent);
    mpz_ui_pow_ui(exponent, 2, t); /// (2^T), where T = 2^40

    sample_prime(l, x);

    /// q = 2^T / l
    mpz_t q;
    mpz_init(q);
    mpz_fdiv_q(q, exponent, l);

    /// pi = x^floor(2^T / l)
    mpz_powm(pi, x, q, N);

    mpz_clear(exponent);
    mpz_clear(q);
    Proof proof_sent = Proof();
    return proof_sent;
}


bool Wesolowski::verifier(mpz_t x, mpz_t y_check, mpz_t l, mpz_t pi) {
    auto start_verif = std::chrono::high_resolution_clock::now();
    /// check: pi^l * x^r == y_check

    mpz_t phi_l;
    mpz_init(phi_l);
    mpz_sub_ui(phi_l, l, 1);

    /// efficient calculation of r: instead of 2^t mod l, we do 2^(t mod phi(l)) mod l
    mpz_t tau_mod;
    mpz_init(tau_mod);
    mpz_set_ui(tau_mod, t);
    mpz_mod(tau_mod, tau_mod, phi_l);

    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2);

    /// x^r, r = 2^T mod l
    /// tau_mod = T mod phi(l)
    /// r = 2^tau_mod mod l
    /// Doesn't make sense: T is much smaller than l.
    mpz_t r;
    mpz_init(r);
    mpz_powm(r, two, tau_mod, l);

    mpz_t y, y_tmp;
    mpz_init(y);
    mpz_init(y_tmp);
    mpz_powm(y, pi, l, N);
    mpz_powm(y_tmp, x, r, N);
    mpz_mul(y, y, y_tmp);
    mpz_mod(y, y, N);

    bool verify_result = (mpz_cmp(y, y_check) == 0);
    auto finish_verif = std::chrono::high_resolution_clock::now();
    verif_time = finish_verif - start_verif;

    mpz_clear(phi_l);
    mpz_clear(r);
    mpz_clear(tau_mod);
    mpz_clear(two);
    mpz_clear(y_tmp);
    mpz_clear(y);
    return verify_result;
}