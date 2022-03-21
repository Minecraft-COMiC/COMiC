#ifndef COMIC_RSA_H
#define COMIC_RSA_H

#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <COMiC/types.h>

typedef struct COMiC_Util_RSA_Keypair
{
    EVP_PKEY *publicKey;
    EVP_PKEY *privateKey;
} COMiC_Util_RSA_Keypair;

COMiC_Constructor
RSA *COMiC_Util_RSA_Init(void);

EVP_PKEY *COMiC_Util_RSA_CreateKey(
        COMiC_In RSA *pRSA
);

void COMiC_Util_RSA_GenerateKeypair(
        COMiC_In COMiC_Util_RSA_Keypair *keypair
);

#endif //COMIC_RSA_H
