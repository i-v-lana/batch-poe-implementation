//
// Created by Светлана Иванова on 02.08.2023.
//

#ifndef SRC_PRF_H
#define SRC_PRF_H


#include "mpz_helper.h"
#include <openssl/rand.h>

class PRF {
public:
    PRF(bigint _k, int _k_bits, int _input_bits, int _output_bits);
    bigint gen(int i);
    bigint PRG(bigint prg_k);
private:
    bigint k;
    int k_bits = 0;
    int input_bits = 0;
    int output_bits = 0;
};


#endif //SRC_PRF_H
