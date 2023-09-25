//
// Created by Светлана Иванова on 24.09.2023.
//

#ifndef SRC_PRF_AES_H
#define SRC_PRF_AES_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include "string.h"
#include "mpz_helper.h"


class PRF_AES {
private:
    std::string key, iv;
    /* Buffer for the tag */
    unsigned char tag[16] = "\0";
    /* Some additional data to be authenticated */
    unsigned char* aad = (unsigned char *) "Some AAD data";
    int output_bits = 0;
public:
    PRF_AES(bigint _k, std::string& _iv, int _output_bits);
    bigint evaluate(bigint& x);

    bigint decrypt(bigint _cipher);
};


#endif //SRC_PRF_AES_H
