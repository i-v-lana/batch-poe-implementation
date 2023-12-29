#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
//#include "sha512.h"
//#include "utils.h"
#include "wesolowski.h"
#include <gmpxx.h>
#include "mpz_helper.h"

void Wesolowski::hash_prime(mpz_t l, const mpz_t input) {
    /// TODO: l by se melo volit pomoci hashovani, jinak bezpecnostni problem

    mpz_t num, gcd, one;
    mpz_init(num);
    mpz_init(gcd);
    mpz_init(one);

    mpz_set_ui(one, 1);
    mpz_urandomb(num, rstate, 2*k);
    mpz_nextprime(l, num);
    mpz_gcd(gcd, l, input);
    while (mpz_cmp(gcd, one)) {
        mpz_urandomb(num, rstate, 2*k);
        mpz_nextprime(l, num);
        mpz_gcd(gcd, l, input);
    }
    mpz_clear(num);
    mpz_clear(gcd);
    mpz_clear(one);
}

Wesolowski::Wesolowski() {
}

void Wesolowski::setup(int _k, const mpz_t& _N) {
    auto start = std::chrono::high_resolution_clock::now();

    k = _k;
    mpz_init(challenge);


    gmp_randinit_mt(rstate);
    mpz_init(N);
    mpz_set(N, _N);

    auto finish = std::chrono::high_resolution_clock::now();
    setup_time = finish - start;
}

Proof Wesolowski::prover_trapdoor(mpz_t l, mpz_t pi, const mpz_t x, const long challenge, const mpz_t phi_N) {
    mpz_t exp_challenge;
    mpz_init(exp_challenge);
    mpz_ui_pow_ui(exp_challenge, 2, challenge);

    hash_prime(l, x);

    mpz_t _q;
    mpz_init(_q);
    mpz_fdiv_q(_q, exp_challenge, l);

    /// Euler's theorem: x ^ q = x ^ (q mod phi(N))
    mpz_fdiv_r(_q, _q, phi_N);
    mpz_powm(pi, x, _q, N);

    mpz_clear(exp_challenge);
    mpz_clear(_q);
    Proof proof_sent = Proof();
    return proof_sent;
}

Proof Wesolowski::prover(mpz_t l, mpz_t pi, const mpz_t x, const long challenge) {
    mpz_t exp_challenge;
    mpz_init(exp_challenge);
    mpz_ui_pow_ui(exp_challenge, 2, challenge); /// (2^T), where T = 2^40

    hash_prime(l, x);

    mpz_t _q;
    mpz_init(_q);
    mpz_fdiv_q(_q, exp_challenge, l);

    mpz_powm(pi, x, _q, N);


    mpz_clear(exp_challenge);
    mpz_clear(_q);
    Proof proof_sent = Proof();
    return proof_sent;
}


bool Wesolowski::verifier(mpz_t x, mpz_t y_check, long challenge, mpz_t l, mpz_t pi) {
    auto start_verif = std::chrono::high_resolution_clock::now();


    mpz_t phi_l;
    mpz_init(phi_l);
    mpz_sub_ui(phi_l, l, 1);

    mpz_t tau_mod;
    mpz_init(tau_mod);
    mpz_set_ui(tau_mod, challenge);
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

    std::chrono::duration<double> exp_time;

    auto start_exp = std::chrono::high_resolution_clock::now();

    mpz_t y, y_tmp;
    mpz_init(y);
    mpz_init(y_tmp);
    mpz_powm(y, pi, l, N);
    mpz_powm(y_tmp, x, r, N);
    mpz_mul(y, y, y_tmp);
    mpz_mod(y, y, N);

    auto finish_exp = std::chrono::high_resolution_clock::now();

    /// TODO: smazat tisk
//    std::cout << "X = " << x << std::endl;
//    std::cout << "PI = " << pi << std::endl;
//    std::cout << "r = " << r << std::endl;
//    std::cout << "l = " << l << std::endl;
//    std::cout << "y = " << y << std::endl;
//    std::cout << "N = " << N << std::endl;
//    std::cout << "CHALLENGE = " << challenge << std::endl;
//    std::cout << "Y_CHECK = " << y_check << std::endl;
    bool verify_result = (mpz_cmp(y, y_check) == 0);
    auto finish_verif = std::chrono::high_resolution_clock::now();
    verif_time = finish_verif - start_verif;

    exp_time = finish_exp - start_exp;
//    std::cout << "EXP : " << exp_time.count() << std::endl;
    mpz_clear(phi_l);
    mpz_clear(r);
    mpz_clear(tau_mod);
    mpz_clear(two);
    mpz_clear(y_tmp);
    mpz_clear(y);
    return verify_result;
}


void exponentiation(mpz_t ret, mpz_t radix, mpz_t exp, mpz_t N)
{
    mpz_powm(ret, radix, exp, N);
}

void exponentiation_euclidian(mpz_t ret, mpz_t radix, mpz_t exp, mpz_t N, int k)
{
    mpz_t A, B;
    mpz_init(A);
    mpz_init(B);

    mpz_t and_mod;
    mpz_init(and_mod);

    for(int i = 0; i < k; i++)
    {
        mpz_setbit(and_mod, i);
    }

    mpz_and(B, N, and_mod);
    mpz_sub(A, N, B);
    mpz_fdiv_q_2exp(A, A, k);

    /*
       std::cout << l << std::endl;
       std::cout << "N = " << N << std::endl;
       std::cout << "A = " << A << std::endl;
       std::cout << "B = " << B << std::endl;
       //mpz_powm(y, pi, l, N);
       //mpz_powm(y_tmp, x, r, N);
     */
    mpz_powm(ret, radix, exp, N);
}


bool Wesolowski::parallel_verify(mpz_t x, long challenge, mpz_t l, mpz_t pi) {

    auto start_verif = std::chrono::high_resolution_clock::now();


    mpz_t phi_l;
    mpz_init(phi_l);
    mpz_sub_ui(phi_l, l, 1);

    mpz_t tau_mod;
    mpz_init(tau_mod);
    mpz_set_ui(tau_mod, challenge);
    mpz_mod(tau_mod, tau_mod, phi_l);

    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2);

    mpz_t r;
    mpz_init(r);
    mpz_powm(r, two, tau_mod, l);

    mpz_t y, y_tmp;
    mpz_init(y);
    mpz_init(y_tmp);

    std::thread first(exponentiation, y, pi, l, N);
    std::thread second(exponentiation, y_tmp, x, r, N);

    first.join();
    second.join();
    //mpz_powm(y, pi, l, N);
    //mpz_powm(y_tmp, x, r, N);
    mpz_mul(y, y, y_tmp);
    mpz_mod(y, y, N);

    hash_prime(l, x);

    if(mpz_cmp(y, y_saved) == 0) {
        auto finish_verif = std::chrono::high_resolution_clock::now();

        verif_time = finish_verif - start_verif;
        //std::cout << verif_time.count() << std::endl;
        return 1;
    } else {
        std::cout << "NOT WORKING" << std::endl;
        exit(1);
        return 0;
    }
}



bool Wesolowski::parallel_diff_verify(mpz_t x, long challenge, mpz_t l, mpz_t pi) {

    auto start_verif = std::chrono::high_resolution_clock::now();


    mpz_t phi_l;
    mpz_init(phi_l);
    mpz_sub_ui(phi_l, l, 1);

    mpz_t tau_mod;
    mpz_init(tau_mod);
    mpz_set_ui(tau_mod, challenge);
    mpz_mod(tau_mod, tau_mod, phi_l);

    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2);

    mpz_t r;
    mpz_init(r);
    mpz_powm(r, two, tau_mod, l);


    mpz_t diff_exp;
    mpz_init(diff_exp);
    mpz_sub(diff_exp, l, r);

    mpz_t y, y_tmp;
    mpz_init(y);
    mpz_init(y_tmp);

    mpz_t w;
    mpz_init(w);
    mpz_mul(w, pi, x);



    std::thread first(exponentiation_euclidian, y, w, r, N, k);
    std::thread second(exponentiation_euclidian, y_tmp, pi, diff_exp, N, k);

    first.join();
    second.join();


    mpz_mul(y, y, y_tmp);
    mpz_mod(y, y, N);

    hash_prime(l, x);

    if(mpz_cmp(y, y_saved) == 0) {
        auto finish_verif = std::chrono::high_resolution_clock::now();

        verif_time = finish_verif - start_verif;
        //std::cout << verif_time.count() << std::endl;
        return 1;
    } else {
        std::cout << "NOT WORKING" << std::endl;
        exit(1);
        return 0;
    }
}


void print_precomp(std::vector<std::vector<mpz_class> > precomp, int pow_w)
{
    for(int i = 0; i<pow_w; i++)
    {
        for(int j = 0; j<pow_w; j++)
        {
            std::cout << "\t" << precomp[i][j];
        }
        std::cout << std::endl;
    }
}

void get_bit(mpz_t ret, mpz_t e, int j)
{
    mpz_fdiv_q_2exp(ret, e, j);
    mpz_mod_ui(ret, ret, 2);
}

bool test_while(mpz_t x, mpz_t y, int j)
{
    if(mpz_tstbit(x, j) == 0 && mpz_tstbit(y, j) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int filter(mpz_t e, int j, int J)
{
    mpz_t ret;
    mpz_init(ret);
    mpz_fdiv_q_2exp(ret, e, J);

    return mpz_fdiv_ui(ret, pow(2, j-J+1));
}



mpz_class multi_exponentiation(mpz_class x, mpz_class y, mpz_t a, mpz_t b, mpz_class N, int k, int w)
{
    int pow_w = pow(2, w);
    std::vector<std::vector<mpz_class> > pc(pow_w, std::vector<mpz_class>(pow_w));

    pc[1][0] = x;
    pc[0][1] = y;
    pc[2][0] = x * x % N;
    pc[0][2] = y * y % N;

    int mid = pow_w/2;
    for(int i = 1; i < mid; i++)
    {
        pc[2*i+1][0] = pc[2*(i-1)+1][0] * pc[2][0] % N;
        pc[0][2*i+1] = pc[0][2*(i-1)+1] * pc[0][2] % N;
    }

    for(int i = 0; i < mid; i++)
    {
        for(int j = 1; j < pow_w; j++)
        {
            pc[j][2*i+1] = pc[j-1][2*i+1] * x % N;
        }
    }

    for(int i = 0; i < mid; i++)
    {
        for(int j = 1; j < mid; j++)
        {
            pc[2*i+1][2*j] = pc[2*i+1][2*j-1] * y % N;
        }
    }

    mpz_class R(1);
    int j = k-1;

    while(j>=0)
    {
        if(mpz_tstbit(a, j) == 0 && mpz_tstbit(b, j) == 0)
        {
            R = (R*R)%N;
            j--;
        }
        else
        {
            int j_new = std::max(j-w, -1);
            int J = j_new+ 1;

            while(test_while(a,b, J) == true)
            {
                J++;
            }
            int A = filter(a, j, J);
            int B = filter(b, j, J);

            while(j>=J)
            {
                R = (R*R)%N;
                j--;
            }

            R = (R*pc[A][B])%N;

            while(j>j_new)
            {
                R = (R*R)%N;
                j--;
            }

        }
    }

    return R;
}


bool Wesolowski::optimized_verify(mpz_t x, long challenge, mpz_t l, mpz_t pi, int w) {
    //std::cout << "OPTIMIZED VERIFICATION" << std::endl;
    //this->unit_test();
    auto start_verif = std::chrono::high_resolution_clock::now();

    mpz_t phi_l;
    mpz_init(phi_l);
    mpz_sub_ui(phi_l, l, 1);

    mpz_t tau_mod;
    mpz_init(tau_mod);
    mpz_set_ui(tau_mod, challenge);
    mpz_mod(tau_mod, tau_mod, phi_l);

    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2);

    mpz_t r;
    mpz_init(r);
    mpz_powm(r, two, tau_mod, l);

    mpz_class xx(x);
    mpz_class yy(pi);
    mpz_class NN(N);


    mpz_class R = multi_exponentiation(xx, yy, r, l, NN, 2*k, w);
    hash_prime(l, x);


    if(mpz_cmp(R.get_mpz_t(), y_saved) == 0) {
        auto finish_verif = std::chrono::high_resolution_clock::now();

        verif_time = finish_verif - start_verif;
        //std::cout << verif_time_opti.count() << std::endl;
        return 1;
    } else {
        std::cout << "NOT WORKING" << std::endl;
        exit(1);
        return 0;
    }
}