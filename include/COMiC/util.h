#ifndef COMIC_UTIL_H
#define COMIC_UTIL_H

#include <openssl/crypto.h>
#include <COMiC/types.h>

# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_Util_JUUID
{
    COMiC_Int64 most_sig_bits;
    COMiC_Int64 least_sig_bits;
} COMiC_Util_JUUID;

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitFromByteArray(
        COMiC_In const char *data
);

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_Init(
        COMiC_In COMiC_Int64 mostSigBits,
        COMiC_In COMiC_Int64 leastSigBits
);

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitRandom(void);

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitNameUUIDFromBytes(
        COMiC_In const char *name
);

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitFromString(
        COMiC_In char *name
);

COMiC_Int32 COMiC_Util_JUUID_GetVersion(
        COMiC_In COMiC_Util_JUUID self
);

COMiC_Int32 COMiC_Util_JUUID_GetVariant(
        COMiC_In COMiC_Util_JUUID self
);

COMiC_Int64 COMiC_Util_JUUID_GetTimestamp(
        COMiC_In COMiC_Util_JUUID self
);

COMiC_Int32 COMiC_Util_JUUID_GetClockSequence(
        COMiC_In COMiC_Util_JUUID self
);

COMiC_Int64 COMiC_Util_JUUID_GetNode(
        COMiC_In COMiC_Util_JUUID self
);

void COMiC_Util_JUUID_ToString(
        COMiC_In COMiC_Util_JUUID self, COMiC_String out
);

COMiC_Int32 COMiC_Util_JUUID_AreEqual(
        COMiC_In COMiC_Util_JUUID first,
        COMiC_In COMiC_Util_JUUID second
);

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

typedef struct
{
    COMiC_USize size;
    COMiC_UInt32 buffer[4];
    COMiC_UInt8 input[64];
    COMiC_UInt8 digest[16];
} COMiC_Util_MD5;

COMiC_Constructor
void COMiC_Util_MD5_Init(
        COMiC_Out COMiC_Util_MD5 *md5
);

void COMiC_Util_MD5_Update(
        COMiC_Out COMiC_Util_MD5 *md5,
        COMiC_In const COMiC_UInt8 *input_buffer,
        COMiC_In COMiC_USize len
);

void COMiC_Util_MD5_Final(
        COMiC_In COMiC_Util_MD5 *md5
);

COMiC_UInt8 *COMiC_Util_MD5_FromString(
        COMiC_In COMiC_String input
);

# ifdef __cplusplus
};
# endif

#endif // COMIC_UTIL_H
