#ifndef COMIC_UTIL_HPP
#define COMIC_UTIL_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <COMiC/core.hpp>
#include <COMiC/crypto.hpp>

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

        explicit UUID(const Byte data[16]) noexcept;

        explicit UUID(const std::string &str) noexcept;

        static UUID random();

        static UUID fromName(const std::string &name) noexcept;

        [[nodiscard]] std::string toString() const noexcept;

        constexpr inline bool operator==(UUID const &other) const noexcept
        {
            return this->lsb == other.lsb && this->msb == other.msb;
        }

        constexpr inline UUID &operator=(UUID const &other) noexcept = default;
    };
}

#endif // COMIC_UTIL_HPP
