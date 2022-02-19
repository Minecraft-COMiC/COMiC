#ifndef COMIC_RSA_H
#define COMIC_RSA_H

#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

typedef struct Keypair
{
    EVP_PKEY *publicKey;
    EVP_PKEY *privateKey;
} Keypair;

RSA *util_rsa_new(void);

EVP_PKEY *util_rsa_create_key(RSA *pRSA);

void util_rsa_generate_keypair(Keypair *keypair);

#endif //COMIC_RSA_H
