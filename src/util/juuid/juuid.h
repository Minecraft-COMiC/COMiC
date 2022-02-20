#ifndef COMIC_JUUID_H
#define COMIC_JUUID_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/md5.h>

typedef struct COMiC_Util_JUUID
{
    long long mostSigBits;
    long long leastSigBits;
} COMiC_Util_JUUID;

COMiC_Util_JUUID COMiC_Util_JUUID_from_byte_array(const char*);

COMiC_Util_JUUID COMiC_Util_JUUID_new(long long, long long);

COMiC_Util_JUUID COMiC_Util_JUUID_random();

COMiC_Util_JUUID COMiC_Util_JUUID_name_uuid_from_bytes(const char);

COMiC_Util_JUUID COMiC_Util_JUUID_from_string(char *);

int COMiC_Util_JUUID_get_version(COMiC_Util_JUUID uuid);

int COMiC_Util_JUUID_get_variant(COMiC_Util_JUUID uuid);

long long COMiC_Util_JUUID_get_timestamp(COMiC_Util_JUUID uuid);

int COMiC_Util_JUUID_get_clock_sequence(COMiC_Util_JUUID uuid);

long long COMiC_Util_JUUID_get_node(COMiC_Util_JUUID uuid);

void COMiC_Util_JUUID_to_string(COMiC_Util_JUUID uuid, char *out);

int COMiC_Util_JUUID_are_equal(COMiC_Util_JUUID first, COMiC_Util_JUUID second);

#endif //COMIC_JUUID_H
