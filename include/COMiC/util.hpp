#ifndef COMIC_UTIL_HPP
#define COMIC_UTIL_HPP

#include <string>
#include <COMiC/core.hpp>

namespace COMiC::Util
{
    class UUID
    {
    private:
        U64 msb = 0, lsb = 0;

    public:

        UUID() = default;

        constexpr inline UUID(U64 mostSigBits, U64 leastSigBits) noexcept: msb(mostSigBits), lsb(leastSigBits)
        {
        };

        constexpr inline explicit UUID(const Byte *data) noexcept
        {
            U64 msb_ = 0, lsb_ = 0;

            for (I32 i = 0; i < 8; i++)
                msb_ = (msb_ << 8) | (data[i] & 0xFF);

            for (I32 i = 8; i < 16; i++)
                lsb_ = (lsb_ << 8) | (data[i] & 0xFF);

            this->msb = msb_;
            this->lsb = lsb_;
        }

        explicit UUID(const std::string &str, bool dashes) noexcept;

        static UUID random();

        static UUID fromName(const char *name) noexcept;

        [[nodiscard]] constexpr inline U32 version() const noexcept;

        [[nodiscard]] constexpr inline U32 variant() const noexcept;

        [[nodiscard]] constexpr inline U64 timestamp() const noexcept;

        [[nodiscard]] constexpr inline U32 clockSequence() const noexcept;

        [[nodiscard]] constexpr inline U64 node() const noexcept;

        [[nodiscard]] std::string toString() const noexcept;

        constexpr inline bool operator==(UUID const &other) const noexcept
        {
            return this->lsb == other.lsb && this->msb == other.msb;
        }

        constexpr inline UUID &operator=(UUID const &other) noexcept = default;

        ~UUID() = default;
    };
}

#endif // COMIC_UTIL_HPP
