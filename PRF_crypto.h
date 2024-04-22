//
// Created by Светлана Иванова on 01.10.2023.
//

#ifndef SRC_PRF_CRYPTO_H
#define SRC_PRF_CRYPTO_H

#include "string.h"
#include "mpz_helper.h"
//#include <crypto++/modes.h>
//#include <crypto++/aes.h>
//#include <crypto++/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>


class PRF_crypto {
private:
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    int output_bits = 0;
public:
    PRF_crypto(bigint _k, std::string& _iv, int _output_bits);
    bigint evaluate(bigint& x);
    bigint decrypt(bigint& _cipher);
    int length();
};


#endif //SRC_PRF_CRYPTO_H
