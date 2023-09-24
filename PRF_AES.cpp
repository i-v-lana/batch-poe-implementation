#include "PRF_AES.h"


PRF_AES::PRF_AES(bigint _k, std::string& _iv, int _output_bits) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    /* Set up the key and iv. Do I need to say to not hard code these in a real application? :-) */

    key = _k.bytes_string();

    /* A 128 bit IV */
    iv = _iv;
    output_bits = _output_bits;
}

unsigned char* string_to_unsigned_chars(const std::string& str) {
    // Allocate memory for the array
    unsigned char* result = new unsigned char[fmax(str.length(), 128)];

    // Copy characters from the string to the array
    for (size_t i = 0; i < str.length(); i++) {
        result[i] = static_cast<unsigned char>(str[i]);
    }

    return result;
}

std::string unsigned_chars_to_string(const unsigned char* data, size_t len) {
    return {reinterpret_cast<const char*>(data), len};
}

void handleErrors(void)
{
    unsigned long errCode;

    printf("An error occurred\n");
    while(errCode = ERR_get_error())
    {
        char *err = ERR_error_string(errCode, NULL);
        printf("%s\n", err);
    }
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *aad,
            int aad_len, unsigned char *key, unsigned char *iv,
            unsigned char *ciphertext, unsigned char *tag)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0, ciphertext_len = 0;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        handleErrors();

    /* Set IV length if default 12 bytes (96 bits) is not appropriate */
    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
        handleErrors();

    /* Initialise key and IV */
    if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    /* Provide any AAD data. This can be called zero or more times as
     * required
     */
    if(aad && aad_len > 0)
    {
        if(1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
            handleErrors();
    }

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(plaintext)
    {
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            handleErrors();

        ciphertext_len = len;
    }

    /* Finalise the encryption. Normally ciphertext bytes may be written at
     * this stage, but this does not occur in GCM mode
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Get the tag */
    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
        handleErrors();

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *aad,
            int aad_len, unsigned char *tag, unsigned char *key, unsigned char *iv,
            unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0, plaintext_len = 0, ret;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the decryption operation. */
    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        handleErrors();

    /* Set IV length. Not necessary if this is 12 bytes (96 bits) */
    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
        handleErrors();

    /* Initialise key and IV */
    if(!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    /* Provide any AAD data. This can be called zero or more times as
     * required
     */
    if(aad && aad_len > 0)
    {
        if(!EVP_DecryptUpdate(ctx, NULL, &len, aad, aad_len))
            handleErrors();
    }

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(ciphertext)
    {
        if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
            handleErrors();

        plaintext_len = len;
    }

    /* Set expected tag value. Works in OpenSSL 1.0.1d and later */
    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
        handleErrors();

    /* Finalise the decryption. A positive return value indicates success,
     * anything else is a failure - the plaintext is not trustworthy.
     */
    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    if(ret > 0)
    {
        /* Success */
        plaintext_len += len;
        return plaintext_len;
    }
    else
    {
        /* Verify failed */
        return -1;
    }
}


bigint PRF_AES::evaluate(bigint &x) {
    /* Message to be encrypted */
    std::string plaintext;
    plaintext = x.bytes_string();

    /* Some additional data to be authenticated */
    static const unsigned char aad[] = "Some AAD data";

    /* Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, dependant on the
     * algorithm and mode
     */
    unsigned char ciphertext_uch[128];

    /* Buffer for the decrypted text */
    unsigned char decryptedtext_uch[128];

    /* Buffer for the tag */
    unsigned char tag[16];

    int decryptedtext_len = 0, ciphertext_len = 0;

    unsigned char* plaintext_uch = string_to_unsigned_chars(plaintext);
    unsigned char* key_uch = string_to_unsigned_chars(key);
    unsigned char* iv_uch = string_to_unsigned_chars(iv);
    /* Encrypt the plaintext */
    ciphertext_len = encrypt(plaintext_uch, strlen(reinterpret_cast<const char *>(plaintext_uch)),
                             const_cast<unsigned char *>(aad), strlen(
            reinterpret_cast<const char *>(aad)), key_uch, iv_uch, ciphertext_uch, tag);
    std::string cipher_text = unsigned_chars_to_string(ciphertext_uch, ciphertext_len);
    bigint result = bigint();
    result.from_bytes(cipher_text, output_bits);
    return result;
//
//        /* Do something useful with the ciphertext here */
//    printf("Ciphertext is:\n");
//    BIO_dump_fp(stdout, ciphertext_uch, ciphertext_len);
//    printf("Tag is:\n");
//    BIO_dump_fp(stdout, tag, 14);
//
//    /* Mess with stuff */
//    /* ciphertext[0] ^= 1; */
//    /* tag[0] ^= 1; */
//
//    /* Decrypt the ciphertext */
//    decryptedtext_len = decrypt(ciphertext_uch, ciphertext_len, const_cast<unsigned char *>(aad), strlen(
//            reinterpret_cast<const char *>(aad)), tag, key_uch, iv_uch, decryptedtext_uch);
//
//    if(decryptedtext_len < 0)
//    {
//        /* Verify error */
//        printf("Decrypted text failed to verify\n");
//    }
//    else
//    {
//        /* Add a NULL terminator. We are expecting printable text */
//        decryptedtext_uch[decryptedtext_len] = '\0';
//
//        /* Show the decrypted text */
//        printf("Decrypted text is:\n");
//        printf("%s\n", decryptedtext_uch);
//    }
//
//    /* Remove error strings */
//    std::string decrypted_text = unsigned_chars_to_string(decryptedtext_uch, decryptedtext_len);
//
//    bigint result = bigint();
//    result.from_bytes(decrypted_text, output_bits);
//    return result;

}