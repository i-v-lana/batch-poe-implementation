//
// Created by Светлана Иванова on 01.10.2023.
//

#include "PRF_crypto.h"
#include <algorithm>

PRF_crypto::PRF_crypto(bigint _k, std::string &_iv, int _output_bits) {
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    std::string key_bytes = _k.bytes_string(CryptoPP::AES::DEFAULT_KEYLENGTH);
    for (int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; ++i) {
        key[i] = static_cast<CryptoPP::byte>(key_bytes[i]);
    }
    int missing_chars = CryptoPP::AES::BLOCKSIZE - _iv.length();
    for (int i = std::max(missing_chars, 0); i < CryptoPP::AES::BLOCKSIZE; ++i) {
        iv[i] = static_cast<CryptoPP::byte>(_iv[i - std::max(missing_chars, 0)]);
    }
    output_bits = _output_bits;
}

bigint PRF_crypto::evaluate(bigint &x) {
    // 15 bytes, because the PRF is run on maximal 15 bytes numbers and it also corresponds to one block
    std::string plaintext = x.bytes_string(15);
    std::string ciphertext;

    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() );
    stfEncryptor.MessageEnd();


    bigint result = bigint(ciphertext, output_bits);
    return result;
}

bigint PRF_crypto::decrypt(bigint& _cipher) {
    std::string decryptedtext;
    std::string ciphertext = _cipher.bytes_string(CryptoPP::AES::BLOCKSIZE);
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();

    bigint result = bigint(decryptedtext);
    return result;
}

int PRF_crypto::length() {
    return output_bits;
}
