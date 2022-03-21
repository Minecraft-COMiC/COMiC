#include "juuid.h"

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitFromByteArray(
        COMiC_In const char *data
)
{
    long long msb = 0, lsb = 0;

    for (int i = 0; i < 8; i++)
        msb = (msb << 8) | (data[i] & 0xff);

    for (int i = 8; i < 16; i++)
        lsb = (lsb << 8) | (data[i] & 0xff);

    return (COMiC_Util_JUUID) {.mostSigBits = msb, .leastSigBits = lsb};
}

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_Init(
        COMiC_In long long mostSigBits,
        COMiC_In  long long leastSigBits
)
{
    return (COMiC_Util_JUUID) {.mostSigBits = mostSigBits, .leastSigBits = leastSigBits};
}

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitRandom(void)
{
    char randomBytes[16];
    RAND_bytes((unsigned char *) randomBytes, 16);

    randomBytes[6] &= 0x0f;
    randomBytes[6] |= 0x40;
    randomBytes[8] &= 0x3f;
    randomBytes[8] |= (char) 0x80;

    return COMiC_Util_JUUID_InitFromByteArray(randomBytes);
}

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitNameUUIDFromBytes(
        COMiC_In const char *name
)
{
    char md5Bytes[16];
    MD5((unsigned char *) name, 16, (unsigned char *) md5Bytes);

    md5Bytes[6] &= 0x0f;
    md5Bytes[6] |= 0x30;
    md5Bytes[8] &= 0x3f;
    md5Bytes[8] |= (char) 0x80;

    return COMiC_Util_JUUID_InitFromByteArray(md5Bytes);
}

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitFromString(
        COMiC_In char *name
)
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

int COMiC_Util_JUUID_GetVersion(
        COMiC_In COMiC_Util_JUUID self
)
{
    return (int) ((self.mostSigBits >> 12) & 0x0f);
}

int COMiC_Util_JUUID_GetVariant(
        COMiC_In COMiC_Util_JUUID self
)
{
    return (int) (((unsigned long long) self.leastSigBits >> (64 - ((unsigned long long) self.leastSigBits >> 62))) &
                  (self.leastSigBits >> 63));
}

long long COMiC_Util_JUUID_GetTimestamp(
        COMiC_In COMiC_Util_JUUID self
)
{
    if (COMiC_Util_JUUID_GetVersion(self) != 1)
        return -1;

    return (self.mostSigBits & 0x0FFFLL) << 48
           | ((self.mostSigBits >> 16) & 0x0FFFFL) << 32
           | (long long) ((unsigned long long) self.mostSigBits >> 32);
}

int COMiC_Util_JUUID_GetClockSequence(
        COMiC_In COMiC_Util_JUUID self
)
{
    if (COMiC_Util_JUUID_GetVersion(self) != 1)
        return -1;

    return (int) ((unsigned long long) (self.leastSigBits & 0x3FFF000000000000L) >> 48);
}

long long COMiC_Util_JUUID_GetNode(
        COMiC_In COMiC_Util_JUUID self
)
{
    if (COMiC_Util_JUUID_GetVersion(self) != 1)
        return -1;

    return self.leastSigBits & 0x0000FFFFFFFFFFFFL;
}

char *Digits(
        COMiC_In long long val,
        COMiC_In int digits
)
{
    long long hi = 1LL << (digits * 4);
    char *res = malloc(digits + 2);

    sprintf(res, "%llx", hi | (val & (hi - 1)));
    res[digits + 1] = 0;

    return (res + 1);
}

void COMiC_Util_JUUID_ToString(
        COMiC_In COMiC_Util_JUUID self, char *out
)
{
    strcpy(out, Digits(self.mostSigBits >> 32, 8));
    out[8] = '-';
    out[9] = 0;
    strcat(out, Digits(self.mostSigBits >> 16, 4));
    out[13] = '-';
    out[14] = 0;
    strcat(out, Digits(self.mostSigBits, 4));
    out[18] = '-';
    out[19] = 0;
    strcat(out, Digits(self.leastSigBits >> 48, 4));
    out[23] = '-';
    out[24] = 0;
    strcat(out, Digits(self.leastSigBits, 12));
    out[36] = 0;
}

int COMiC_Util_JUUID_AreEqual(
        COMiC_In COMiC_Util_JUUID first,
        COMiC_In COMiC_Util_JUUID second
)
{
    return (first.mostSigBits == second.mostSigBits && first.leastSigBits == second.leastSigBits);
}