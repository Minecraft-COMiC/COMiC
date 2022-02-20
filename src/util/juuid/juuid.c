#include "juuid.h"

COMiC_Util_JUUID COMiC_Util_JUUID_from_byte_array(const char *data)
{
    long long msb = 0;
    long long lsb = 0;

    for (int i = 0; i < 8; i++)
        msb = (msb << 8) | (data[i] & 0xff);

    for (int i = 8; i < 16; i++)
        lsb = (lsb << 8) | (data[i] & 0xff);

    return (COMiC_Util_JUUID) {.mostSigBits = msb, .leastSigBits = lsb};
}

COMiC_Util_JUUID COMiC_Util_JUUID_new(long long mostSigBits, long long leastSigBits)
{
    return (COMiC_Util_JUUID) {.mostSigBits = mostSigBits, .leastSigBits = leastSigBits};
}

COMiC_Util_JUUID COMiC_Util_JUUID_random()
{
    char randomBytes[16];
    RAND_bytes((unsigned char *) randomBytes, 16);

    randomBytes[6] &= 0x0f;
    randomBytes[6] |= 0x40;
    randomBytes[8] &= 0x3f;
    randomBytes[8] |= (char) 0x80;

    return COMiC_Util_JUUID_from_byte_array(randomBytes);
}

COMiC_Util_JUUID COMiC_Util_JUUID_name_uuid_from_bytes(const char)
{
    char md5Bytes[16];
    MD5((unsigned char *) name, 16, (unsigned char *) md5Bytes);

    md5Bytes[6] &= 0x0f;
    md5Bytes[6] |= 0x30;
    md5Bytes[8] &= 0x3f;
    md5Bytes[8] |= (char) 0x80;

    return COMiC_Util_JUUID_from_byte_array(md5Bytes);
}

COMiC_Util_JUUID COMiC_Util_JUUID_from_string(char *name)
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

    return (COMiC_Util_JUUID) {.leastSigBits = leastSigBits, .mostSigBits = mostSigBits};
}

int COMiC_Util_JUUID_get_version(COMiC_Util_JUUID uuid)
{
    return (int) ((uuid.mostSigBits >> 12) & 0x0f);
}

int COMiC_Util_JUUID_get_variant(COMiC_Util_JUUID uuid)
{
    return (int) (((unsigned long long) uuid.leastSigBits >> (64 - ((unsigned long long) uuid.leastSigBits >> 62))) &
                  (uuid.leastSigBits >> 63));
}

long long COMiC_Util_JUUID_get_timestamp(COMiC_Util_JUUID uuid)
{
    if (COMiC_Util_JUUID_get_version(uuid) != 1)
        return -1;

    return (uuid.mostSigBits & 0x0FFFLL) << 48
           | ((uuid.mostSigBits >> 16) & 0x0FFFFL) << 32
           | (long long) ((unsigned long long) uuid.mostSigBits >> 32);
}

int COMiC_Util_JUUID_get_clock_sequence(COMiC_Util_JUUID uuid)
{
    if (COMiC_Util_JUUID_get_version(uuid) != 1)
        return -1;

    return (int) ((unsigned long long) (uuid.leastSigBits & 0x3FFF000000000000L) >> 48);
}

long long COMiC_Util_JUUID_get_node(COMiC_Util_JUUID uuid)
{
    if (COMiC_Util_JUUID_get_version(uuid) != 1)
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

void COMiC_Util_JUUID_to_string(COMiC_Util_JUUID uuid, char *out)
{
    strcpy(out, digits(uuid.mostSigBits >> 32, 8));
    out[8] = '-';
    out[9] = 0;
    strcat(out, digits(uuid.mostSigBits >> 16, 4));
    out[13] = '-';
    out[14] = 0;
    strcat(out, digits(uuid.mostSigBits, 4));
    out[18] = '-';
    out[19] = 0;
    strcat(out, digits(uuid.leastSigBits >> 48, 4));
    out[23] = '-';
    out[24] = 0;
    strcat(out, digits(uuid.leastSigBits, 12));
    out[36] = 0;
}

int COMiC_Util_JUUID_are_equal(COMiC_Util_JUUID first, COMiC_Util_JUUID second)
{
    return (first.mostSigBits == second.mostSigBits && first.leastSigBits == second.leastSigBits);
}