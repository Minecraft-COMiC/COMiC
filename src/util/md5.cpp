#include <cstring>
#include <COMiC/util.hpp>

namespace COMiC::Util
{
    constexpr U64 A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D = 0x10325476;

    static U32 S[] = {
            7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
            5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    static U32 K[] = {
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

    static U8 PADDING[] = {
            0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };


    U32 RotateLeft(U32 x, U32 n)
    {
        return (x << n) | (x >> (32 - n));
    }

    void step(U32 *buffer, U32 *input)
    {
        U32 AA = buffer[0];
        U32 BB = buffer[1];
        U32 CC = buffer[2];
        U32 DD = buffer[3];

        U32 E, j;

        for (U32 i = 0; i < 64; i++)
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

            U32 temp = DD;
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

    MD5::MD5()
    {
        this->size = 0;
        this->buffer[0] = A;
        this->buffer[1] = B;
        this->buffer[2] = C;
        this->buffer[3] = D;
    }

    void MD5::update(const U8 *input_buffer, USize len)
    {
        U32 input_[16];
        U32 offset = this->size % 64;
        this->size += len;

        for (U32 i = 0; i < len; i++)
        {
            this->input[offset++] = (U8) *(input_buffer + i);

            if (offset % 64 == 0)
            {
                for (U32 j = 0; j < 16; j++)
                    input_[j] = (U32) (this->input[(j * 4) + 3]) << 24 |
                                (U32) (this->input[(j * 4) + 2]) << 16 |
                                (U32) (this->input[(j * 4) + 1]) << 8 |
                                (U32) (this->input[(j * 4)]);

                step(this->buffer, input_);
                offset = 0;
            }
        }
    }

    void MD5::final()
    {
        U32 input_[16], offset = this->size % 64, padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

        update(PADDING, padding_length);
        this->size -= padding_length;

        for (I32 j = 0; j < 14; j++)
            input_[j] = (this->input[(j * 4) + 3]) << 24 |
                        (this->input[(j * 4) + 2]) << 16 |
                        (this->input[(j * 4) + 1]) << 8 |
                        this->input[(j * 4)];

        input_[14] = this->size * 8;
        input_[15] = (this->size * 8) >> 32;

        step(this->buffer, input_);

        for (I32 i = 0; i < 4; i++)
        {
            this->digest[(i * 4) + 0] = this->buffer[i] & 0x000000FF;
            this->digest[(i * 4) + 1] = (this->buffer[i] & 0x0000FF00) >> 8;
            this->digest[(i * 4) + 2] = (this->buffer[i] & 0x00FF0000) >> 16;
            this->digest[(i * 4) + 3] = (this->buffer[i] & 0xFF000000) >> 24;
        }
    }

    void MD5::hash(const char *str, U8 out[16])
    {
        update((U8 *) input, strlen(str));
        final();

        memcpy(out, this->digest, 16);
    }

}
