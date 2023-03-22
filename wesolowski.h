#ifndef WESOLOWSKI_H
#define WESOLOWSKI_H

#include </usr/local/include/gmpxx.h>
#include <vector>
#include <chrono>

#include "proof.h"

class Wesolowski {
public:
Wesolowski();

void setup(int lambda, int k);
void generate(mpz_class& dest);
Proof evaluate(mpz_class l, mpz_class pi, const mpz_class x,
               long challenge);
bool parallel_verify(mpz_class x, long challenge, mpz_class l, mpz_class pi);
bool parallel_diff_verify(mpz_class x, long challenge, mpz_class l, mpz_class pi);
bool naive_verify(mpz_class x, long challenge, mpz_class l, mpz_class pi);
bool optimized_verify(mpz_class x, long challenge, mpz_class l, mpz_class pi, int w);
std::chrono::duration<double> setup_time;
std::chrono::duration<double> eval_time;
std::chrono::duration<double> proof_time;
std::chrono::duration<double> verif_time;
std::chrono::duration<double> verif_time_opti;

private:
mpz_class y_saved;
mpz_class N;
mpz_class p;
mpz_class q;
int k;
int lambda;
mpz_class challenge;

gmp_randstate_t rstate;

void HashG(mpz_class& dest, mpz_class hashed);
void hash_prime(mpz_class l, const mpz_class input);

void unit_test();

int bit_length;
};

#endif
