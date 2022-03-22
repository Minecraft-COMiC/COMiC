#include <string.h>
#include <COMiC/util.h>

#define A (0x67452301)
#define B (0xefcdab89)
#define C (0x98badcfe)
#define D (0x10325476)

static COMiC_UInt32 S[] =
    {
            7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
            5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

static COMiC_UInt32 K[] =
    {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
            0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
            0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
            0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

#define F(X, Y, Z) (((X) & ((Y))) | (~(X) & ((Z))))
#define G(X, Y, Z) (((X) & (Z)) | ((Y) & ~(Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | ~(Z)))

static COMiC_UInt8 PADDING[] =
    {
            0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

COMiC_UInt32 RotateLeft(COMiC_UInt32 x, COMiC_UInt32 n)
{
    return (x << n) | (x >> (32 - n));
}

void MD5Step(COMiC_UInt32 *buffer, COMiC_UInt32 *input)
{
    COMiC_UInt32 AA = buffer[0];
    COMiC_UInt32 BB = buffer[1];
    COMiC_UInt32 CC = buffer[2];
    COMiC_UInt32 DD = buffer[3];

    COMiC_UInt32 E, j;

    for (COMiC_UInt32 i = 0; i < 64; i++)
    {
        switch (i / 16)
        {
            case 0:
                E = F(BB, CC, DD);
                j = i;
                break;
            case 1:
                E = G(BB, CC, DD);
                j = ((i * 5) + 1) % 16;
                break;
            case 2:
                E = H(BB, CC, DD);
                j = ((i * 3) + 5) % 16;
                break;
            default:
                E = I(BB, CC, DD);
                j = (i * 7) % 16;
                break;
        }

        COMiC_UInt32 temp = DD;
        DD = CC;
        CC = BB;
        BB = BB + RotateLeft(AA + E + K[i] + input[j], S[i]);
        AA = temp;
    }

    buffer[0] += AA;
    buffer[1] += BB;
    buffer[2] += CC;
    buffer[3] += DD;
}

COMiC_Constructor
void COMiC_Util_MD5_Init(
        COMiC_Out COMiC_Util_MD5 *md5
)
{
    md5->size = 0;
    md5->buffer[0] = A;
    md5->buffer[1] = B;
    md5->buffer[2] = C;
    md5->buffer[3] = D;
}

void COMiC_Util_MD5_Update(
        COMiC_Out COMiC_Util_MD5 *md5,
        COMiC_In const COMiC_UInt8 *input_buffer,
        COMiC_In COMiC_USize len
)
{
    COMiC_UInt32 input[16];
    COMiC_UInt32 offset = md5->size % 64;
    md5->size += len;

    for (COMiC_UInt32 i = 0; i < len; i++)
    {
        md5->input[offset++] = (COMiC_UInt8) *(input_buffer + i);

        if (offset % 64 == 0)
        {
            for (COMiC_UInt32 j = 0; j < 16; j++)
                input[j] = (COMiC_UInt32) (md5->input[(j * 4) + 3]) << 24 |
                           (COMiC_UInt32) (md5->input[(j * 4) + 2]) << 16 |
                           (COMiC_UInt32) (md5->input[(j * 4) + 1]) << 8 |
                           (COMiC_UInt32) (md5->input[(j * 4)]);

            MD5Step(md5->buffer, input);
            offset = 0;
        }
    }
}

void COMiC_Util_MD5_Final(
        COMiC_In COMiC_Util_MD5 *md5
)
{
    COMiC_UInt32 input[16], offset = md5->size % 64, padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

    COMiC_Util_MD5_Update(md5, PADDING, padding_length);
    md5->size -= (uint64_t) padding_length;

    for (COMiC_UInt32 j = 0; j < 14; j++)
        input[j] = (md5->input[(j * 4) + 3]) << 24 |
                   (md5->input[(j * 4) + 2]) << 16 |
                   (md5->input[(j * 4) + 1]) << 8  |
                   md5->input[(j * 4)];

    input[14] = md5->size * 8;
    input[15] = (md5->size * 8) >> 32;

    MD5Step(md5->buffer, input);

    for (COMiC_UInt32 i = 0; i < 4; i++)
    {
        md5->digest[(i * 4) + 0] = md5->buffer[i] & 0x000000FF;
        md5->digest[(i * 4) + 1] = (md5->buffer[i] & 0x0000FF00) >> 8;
        md5->digest[(i * 4) + 2] = (md5->buffer[i] & 0x00FF0000) >> 16;
        md5->digest[(i * 4) + 3] = (md5->buffer[i] & 0xFF000000) >> 24;
    }
}

COMiC_UInt8 *COMiC_Util_MD5_FromString(
        COMiC_In COMiC_String input
)
{
    COMiC_Util_MD5 md5;
    COMiC_Util_MD5_Init(&md5);
    COMiC_Util_MD5_Update(&md5, (COMiC_UInt8 *) input, strlen(input));
    COMiC_Util_MD5_Final(&md5);

    COMiC_UInt8 *result = malloc(16);
    memcpy(result, md5.digest, 16);
    return result;
}