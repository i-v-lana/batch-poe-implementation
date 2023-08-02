//
// Created by Светлана Иванова on 02.08.2023.
//

#include <bitset>
#include "PRF.h"
#include <iostream>

const int MAX_CNT_BITS = 1000;

PRF::PRF(bigint _k, int _k_bits, int _input_bits, int _output_bits) {
    k = _k;
    k_bits = _k_bits;
    input_bits = _input_bits;
    output_bits = _output_bits;
}

bigint PRF::gen(int x) {
    std::bitset<MAX_CNT_BITS> bits(x);
    bigint cur = bigint(k.num);
    for (int i = input_bits - 1; i >= 0; --i) {
        std::cout << cur.num << " cur bit: " << bits[i] << std::endl;
        cur = PRG(cur);
        /// the length of the PRG result
        if (bits[i])
            cur = cur.last_half(k_bits * 2);
        else
            cur = cur.first_half(k_bits * 2);
    }
    std::cout << cur.num << std::endl;
    /// still need to take only first log s bits
    return cur.first_n_bits(output_bits);
}

bigint PRF::PRG(bigint prg_k) {
    RAND_seed(prg_k.num, sizeof(prg_k.num));
    int out_bytes = k_bits * 2 / 8;
    std::cout << out_bytes << " bytes generated" << std::endl;
    unsigned char random_bytes[out_bytes];
    RAND_bytes(random_bytes, out_bytes);
    bigint ans = bigint();
    mpz_import(ans.num, sizeof(random_bytes), 1, sizeof(random_bytes[0]), 0, 0, random_bytes);
    std::cout << "generated " << ans.num << std::endl;
    return ans;
}
