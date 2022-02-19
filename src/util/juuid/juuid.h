#ifndef COMIC_JUUID_H
#define COMIC_JUUID_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/md5.h>

typedef struct JUUID
{
    long long mostSigBits;
    long long leastSigBits;
} JUUID;

JUUID util_uuid_from_byte_array(const char*);

JUUID util_uuid_new(long long, long long);

JUUID util_uuid_random();

JUUID util_uuid_name_uuid_from_bytes(const char[16]);

JUUID util_uuid_from_string(char *);

int util_uuid_get_version(JUUID uuid);

int util_uuid_get_variant(JUUID uuid);

long long util_uuid_get_timestamp(JUUID uuid);

int util_uuid_get_clock_sequence(JUUID uuid);

long long util_uuid_get_node(JUUID uuid);

void util_uuid_to_string(JUUID uuid, char  [37]);

int util_uuid_are_equal(JUUID first, JUUID second);

#endif //COMIC_JUUID_H
