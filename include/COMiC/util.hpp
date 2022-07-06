#ifndef COMIC_UTIL_HPP
#define COMIC_UTIL_HPP

#include <COMiC/core.hpp>

namespace COMiC::Util
{
    class UUID
    {
    private:
        I64 msb;
        I64 lsb;

    public:

        constexpr inline UUID(I64 mostSigBits, I64 leastSigBits) noexcept: msb(mostSigBits), lsb(leastSigBits)
        {
        };

        constexpr inline explicit UUID(const char data[16]) noexcept: msb(), lsb()
        {
            I64 msb_ = 0, lsb_ = 0;

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

        [[nodiscard]] constexpr inline I32 version() const noexcept;

        [[nodiscard]] constexpr inline I32 variant() const noexcept;

        [[nodiscard]] constexpr inline I64 timestamp() const noexcept;

        [[nodiscard]] constexpr inline I32 clockSequence() const noexcept;

        [[nodiscard]] constexpr inline I64 node() const noexcept;

        inline void toString(char out[37]) const noexcept;

        constexpr inline bool operator==(UUID const &other) const noexcept
        {
            return this->lsb == other.lsb && this->msb == other.msb;
        }

        constexpr inline UUID& operator=(UUID const &other) noexcept = default;

        inline ~UUID() noexcept = default;
    };

    class MD5
    {
    private:
        USize size;
        U32 buffer[4]{};
        U8 input[64]{};
        U8 digest[16]{};
    public:
        constexpr inline MD5() noexcept;

        void update(const U8 *input_buffer, USize len);

        void final();

        void hash(const char *str, U8 out[16]);

        ~MD5() = default;
    };
}

#endif // COMIC_UTIL_HPP
