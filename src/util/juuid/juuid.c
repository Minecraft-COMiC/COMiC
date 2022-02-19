#include "juuid.h"

JUUID util_uuid_from_byte_array(const char *data)
{
    long long msb = 0;
    long long lsb = 0;

    for (int i = 0; i < 8; i++)
        msb = (msb << 8) | (data[i] & 0xff);

    for (int i = 8; i < 16; i++)
        lsb = (lsb << 8) | (data[i] & 0xff);

    return (JUUID) {.mostSigBits = msb, .leastSigBits = lsb};
}

JUUID util_uuid_new(long long mostSigBits, long long leastSigBits)
{
    return (JUUID) {.mostSigBits = mostSigBits, .leastSigBits = leastSigBits};
}

JUUID util_uuid_random()
{
    char randomBytes[16];
    RAND_bytes((unsigned char *) randomBytes, 16);

    randomBytes[6] &= 0x0f;
    randomBytes[6] |= 0x40;
    randomBytes[8] &= 0x3f;
    randomBytes[8] |= (char) 0x80;

    return util_uuid_from_byte_array(randomBytes);
}

JUUID util_uuid_name_uuid_from_bytes(const char name[16])
{
    char md5Bytes[16];
    MD5((unsigned char *) name, 16, (unsigned char *) md5Bytes);

    md5Bytes[6] &= 0x0f;
    md5Bytes[6] |= 0x30;
    md5Bytes[8] &= 0x3f;
    md5Bytes[8] |= (char) 0x80;

    return util_uuid_from_byte_array(md5Bytes);
}

JUUID util_uuid_from_string(char *name)
{
    const char *delim = "-";
    char *token = strtok(name, delim);
    char *components[5];

    for (int i = 0; i < 5; i++)
    {
        components[i] = token;
        token = strtok(NULL, delim);
    }

    long long mostSigBits = strtoll(components[0], NULL, 16);
    mostSigBits <<= 16;
    mostSigBits |= strtoll(components[1], NULL, 16);
    mostSigBits <<= 16;
    mostSigBits |= strtoll(components[2], NULL, 16);

    long long leastSigBits = strtoll(components[3], NULL, 16);
    leastSigBits <<= 48;
    leastSigBits |= strtoll(components[4], NULL, 16);

    return (JUUID) {.leastSigBits = leastSigBits, .mostSigBits = mostSigBits};
}

int util_uuid_get_version(JUUID uuid)
{
    return (int) ((uuid.mostSigBits >> 12) & 0x0f);
}

int util_uuid_get_variant(JUUID uuid)
{
    return (int) (((unsigned long long) uuid.leastSigBits >> (64 - ((unsigned long long) uuid.leastSigBits >> 62))) &
                  (uuid.leastSigBits >> 63));
}

long long util_uuid_get_timestamp(JUUID uuid)
{
    if (util_uuid_get_version(uuid) != 1)
        return -1;

    return (uuid.mostSigBits & 0x0FFFLL) << 48
           | ((uuid.mostSigBits >> 16) & 0x0FFFFL) << 32
           | (long long) ((unsigned long long) uuid.mostSigBits >> 32);
}

int util_uuid_get_clock_sequence(JUUID uuid)
{
    if (util_uuid_get_version(uuid) != 1)
        return -1;

    return (int) ((unsigned long long) (uuid.leastSigBits & 0x3FFF000000000000L) >> 48);
}

long long util_uuid_get_node(JUUID uuid)
{
    if (util_uuid_get_version(uuid) != 1)
        return -1;

    return uuid.leastSigBits & 0x0000FFFFFFFFFFFFL;
}

char *digits(long long val, int digits)
{
    long long hi = 1LL << (digits * 4);
    char *res = malloc(digits + 2);

    sprintf(res, "%llx", hi | (val & (hi - 1)));
    res[digits + 1] = 0;

    return (res + 1);
}

void util_uuid_to_string(JUUID uuid, char out[37])
{
    strcpy(out, digits(uuid.mostSigBits >> 32, 8));
    out[8] = '-'; out[9] = 0;
    strcat(out, digits(uuid.mostSigBits >> 16, 4));
    out[13] = '-'; out[14] = 0;
    strcat(out, digits(uuid.mostSigBits, 4));
    out[18] = '-'; out[19] = 0;
    strcat(out, digits(uuid.leastSigBits >> 48, 4));
    out[23] = '-'; out[24] = 0;
    strcat(out, digits(uuid.leastSigBits, 12));
    out[36] = 0;
}

int util_uuid_are_equal(JUUID first, JUUID second)
{
    return (first.mostSigBits == second.mostSigBits && first.leastSigBits == second.leastSigBits);
}