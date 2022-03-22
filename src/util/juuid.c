#include <COMiC.h>
#include <openssl/rand.h>
#include <openssl/md5.h>

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_InitFromByteArray(
        COMiC_In const char *data
)
{
    COMiC_Int64 msb = 0, lsb = 0;

    for (COMiC_Int32 i = 0; i < 8; i++)
        msb = (msb << 8) | (data[i] & 0xff);

    for (COMiC_Int32 i = 8; i < 16; i++)
        lsb = (lsb << 8) | (data[i] & 0xff);

    return (COMiC_Util_JUUID) {.most_sig_bits = msb, .least_sig_bits = lsb};
}

COMiC_Constructor
COMiC_Util_JUUID COMiC_Util_JUUID_Init(
        COMiC_In COMiC_Int64 most_sig_bits,
        COMiC_In  COMiC_Int64 least_sig_bits
)
{
    return (COMiC_Util_JUUID) {.most_sig_bits = most_sig_bits, .least_sig_bits = least_sig_bits};
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

    for (COMiC_Int32 i = 0; i < 5; i++)
    {
        components[i] = token;
        token = strtok(NULL, delim);
    }

    COMiC_Int64 most_sig_bits = strtoll(components[0], NULL, 16);
    most_sig_bits <<= 16;
    most_sig_bits |= strtoll(components[1], NULL, 16);
    most_sig_bits <<= 16;
    most_sig_bits |= strtoll(components[2], NULL, 16);

    COMiC_Int64 least_sig_bits = strtoll(components[3], NULL, 16);
    least_sig_bits <<= 48;
    least_sig_bits |= strtoll(components[4], NULL, 16);

    return (COMiC_Util_JUUID) {.least_sig_bits = least_sig_bits, .most_sig_bits = most_sig_bits};
}

COMiC_Int32 COMiC_Util_JUUID_GetVersion(
        COMiC_In COMiC_Util_JUUID self
)
{
    return (COMiC_Int32) ((self.most_sig_bits >> 12) & 0x0f);
}

COMiC_Int32 COMiC_Util_JUUID_GetVariant(
        COMiC_In COMiC_Util_JUUID self
)
{
    return (COMiC_Int32) (((COMiC_UInt64) self.least_sig_bits >> (64 - ((COMiC_UInt64) self.least_sig_bits >> 62))) &
                  (self.least_sig_bits >> 63));
}

COMiC_Int64 COMiC_Util_JUUID_GetTimestamp(
        COMiC_In COMiC_Util_JUUID self
)
{
    if (COMiC_Util_JUUID_GetVersion(self) != 1)
        return -1;

    return (self.most_sig_bits & 0x0FFFLL) << 48
           | ((self.most_sig_bits >> 16) & 0x0FFFFL) << 32
           | (COMiC_Int64) ((COMiC_UInt64) self.most_sig_bits >> 32);
}

COMiC_Int32 COMiC_Util_JUUID_GetClockSequence(
        COMiC_In COMiC_Util_JUUID self
)
{
    if (COMiC_Util_JUUID_GetVersion(self) != 1)
        return -1;

    return (COMiC_Int32) ((COMiC_UInt64) (self.least_sig_bits & 0x3FFF000000000000L) >> 48);
}

COMiC_Int64 COMiC_Util_JUUID_GetNode(
        COMiC_In COMiC_Util_JUUID self
)
{
    if (COMiC_Util_JUUID_GetVersion(self) != 1)
        return -1;

    return self.least_sig_bits & 0x0000FFFFFFFFFFFFL;
}

char *Digits(
        COMiC_In COMiC_Int64 val,
        COMiC_In COMiC_Int32 digits
)
{
    COMiC_Int64 hi = 1LL << (digits * 4);
    char *res = malloc(digits + 2);

    sprintf(res, "%llx", hi | (val & (hi - 1)));
    res[digits + 1] = 0;

    return (res + 1);
}

void COMiC_Util_JUUID_ToString(
        COMiC_In COMiC_Util_JUUID self, COMiC_String out
)
{
    strcpy(out, Digits(self.most_sig_bits >> 32, 8));
    out[8] = '-';
    out[9] = 0;
    strcat(out, Digits(self.most_sig_bits >> 16, 4));
    out[13] = '-';
    out[14] = 0;
    strcat(out, Digits(self.most_sig_bits, 4));
    out[18] = '-';
    out[19] = 0;
    strcat(out, Digits(self.least_sig_bits >> 48, 4));
    out[23] = '-';
    out[24] = 0;
    strcat(out, Digits(self.least_sig_bits, 12));
    out[36] = 0;
}

COMiC_Int32 COMiC_Util_JUUID_AreEqual(
        COMiC_In COMiC_Util_JUUID first,
        COMiC_In COMiC_Util_JUUID second
)
{
    return (first.most_sig_bits == second.most_sig_bits && first.least_sig_bits == second.least_sig_bits);
}