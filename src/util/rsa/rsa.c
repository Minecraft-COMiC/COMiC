#include "rsa.h"

RSA *util_rsa_new(void)
{
    BIGNUM *bne = BN_new();
    BN_set_word(bne, RSA_F4);
    RSA *rsa = RSA_new();
    RSA_generate_key_ex(rsa, 2048, bne, NULL);

    return rsa;
}

EVP_PKEY *util_rsa_create_key(RSA *pRSA)
{
    EVP_PKEY *pKey = EVP_PKEY_new();
    if (pRSA && pKey && EVP_PKEY_assign_RSA(pKey, pRSA))
    {
        if (RSA_check_key(pRSA) <= 0)
        {
            EVP_PKEY_free(pKey);
            pKey = NULL;
        }
    }
    else
    {
        if (pRSA)
        {
            RSA_free(pRSA);
            pRSA = NULL;
        }

        if (pKey)
        {
            EVP_PKEY_free(pKey);
            pKey = NULL;
        }
    }

    return pKey;
}

void util_rsa_generate_keypair(Keypair *keypair)
{
    printf("Generating RSA keypair... ");

    RSA *rsa = util_rsa_new();
    if ((keypair->privateKey = util_rsa_create_key(rsa)) == NULL || (keypair->publicKey = util_rsa_create_key(rsa)) == NULL)
    {
        printf("%s", ERR_error_string(ERR_get_error(), NULL));
        exit(1);
    }
    puts("Done");
}
