#ifndef COMIC_JUUID_H
#define COMIC_JUUID_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/md5.h>
#include "COMiC/types.h"

typedef struct COMiC_Util_JUUID
{
    long long mostSigBits;
    long long leastSigBits;
} COMiC_Util_JUUID;

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitFromByteArray(
        COMiC_In const char *data
);

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_Init(
        COMiC_In long long mostSigBits,
        COMiC_In  long long leastSigBits
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

int COMiC_Util_JUUID_GetVersion(
        COMiC_In COMiC_Util_JUUID self
);

int COMiC_Util_JUUID_GetVariant(
        COMiC_In COMiC_Util_JUUID self
);

long long COMiC_Util_JUUID_GetTimestamp(
        COMiC_In COMiC_Util_JUUID self
);

int COMiC_Util_JUUID_GetClockSequence(
        COMiC_In COMiC_Util_JUUID self
);

long long COMiC_Util_JUUID_GetNode(
        COMiC_In COMiC_Util_JUUID self
);

void COMiC_Util_JUUID_ToString(
        COMiC_In COMiC_Util_JUUID self, char *out
);

int COMiC_Util_JUUID_AreEqual(
        COMiC_In COMiC_Util_JUUID first,
        COMiC_In COMiC_Util_JUUID second
);

#endif //COMIC_JUUID_H
