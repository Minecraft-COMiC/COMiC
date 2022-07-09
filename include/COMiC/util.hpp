#ifndef COMIC_UTIL_HPP
#define COMIC_UTIL_HPP

#include <COMiC/core.hpp>

namespace COMiC::Util
{
    class UUID
    {
    private:
        U64 msb = 0, lsb = 0;

    public:

        constexpr inline UUID(U64 mostSigBits, U64 leastSigBits) noexcept: msb(mostSigBits), lsb(leastSigBits)
        {
        };

        constexpr inline explicit UUID(const Byte data[16]) noexcept
        {
            U64 msb_ = 0, lsb_ = 0;

            for (I32 i = 0; i < 8; i++)
                msb_ = (msb_ << 8) | (data[i] & 0xFF);

            for (I32 i = 8; i < 16; i++)
                lsb_ = (lsb_ << 8) | (data[i] & 0xFF);

            this->msb = msb_;
            this->lsb = lsb_;
        }

        static UUID random();

        static UUID fromName(const char *name) noexcept;

        static UUID fromString(const char *str) noexcept;

        [[nodiscard]] constexpr inline U32 version() const noexcept;

        [[nodiscard]] constexpr inline U32 variant() const noexcept;

        [[nodiscard]] constexpr inline U64 timestamp() const noexcept;

        [[nodiscard]] constexpr inline U32 clockSequence() const noexcept;

        [[nodiscard]] constexpr inline U64 node() const noexcept;

        void toString(char out[37]) const noexcept;

        constexpr inline bool operator==(UUID const &other) const noexcept
        {
            return this->lsb == other.lsb && this->msb == other.msb;
        }

        constexpr inline UUID &operator=(UUID const &other) noexcept = default;

        ~UUID() = default;
    };

    class MD5
    {
    private:
        USize size = 0;
        U32 buffer[4]{};
        U8 input[64]{};
        Byte digest[16]{};
    public:
        constexpr MD5() noexcept;

        void update(const Byte *input_buffer, USize len);

        void final();

        static void hash(const char *str, Byte out[16]);

        ~MD5() = default;
    };

    class SHA1
    {
    private:
        U32 state[5]{};
        U32 count[2]{};
        Byte buffer[64]{};

    public:
        SHA1()
        {
            this->state[0] = 0x67452301;
            this->state[1] = 0xEFCDAB89;
            this->state[2] = 0x98BADCFE;
            this->state[3] = 0x10325476;
            this->state[4] = 0xC3D2E1F0;

            this->count[0] = 0;
            this->count[1] = 0;
        }

        void update(const Byte *data, size_t len);

        void final(Byte digest[20]);

//        void hexDigest(char digest[20], char *out);
    };

    class AES
    {
    private:
        static const U8 AES_ROUNDS = 10, KEY_SIZE = 16, BLOCK_SIZE = 16, BUFFER_LENGTH = BLOCK_SIZE * 2,
                V_LENGTH = 16, K_LENGTH = 16, EXP_KEY_SIZE = 4 * (AES_ROUNDS + 1);

        static const I8 S[256], rcon[30];
        static const I32 T1[256], T2[256], T3[256], T4[256];

        I8 key[KEY_SIZE]{}, v[V_LENGTH]{}, k[K_LENGTH]{}, buffer[BUFFER_LENGTH]{};
        I32 encryptionSubKey[EXP_KEY_SIZE]{};
        size_t buffered = 0, diffBlockSize = BLOCK_SIZE;

        void encrypt_block(const I8 in[16], I8 out[16]);

        size_t enc(const I8 *plain, size_t plainOffset, size_t plainLen, I8 *cipher, size_t cipherOffset);

        size_t dec(const I8 *cipher, size_t cipherOffset, size_t cipherLen, I8 *plain, size_t plainOffset);

        size_t update(const I8 *input, size_t inputLen, I8 *output, bool decrypting);

    public:
        AES(const I8 encoded[KEY_SIZE], const I8 iv[V_LENGTH]);

        void encrypt(const I8 *in, size_t in_size, I8 *out, size_t *out_size);

        void decrypt(const I8 *in, size_t in_size, I8 *out, size_t *out_size);
    };
}

#endif // COMIC_UTIL_HPP
