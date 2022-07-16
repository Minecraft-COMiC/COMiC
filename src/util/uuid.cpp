#include <charconv>
#include <sstream>
#include <algorithm>
#include <COMiC/util.hpp>
#include <COMiC/crypto.hpp>

namespace COMiC::Util
{
    UUID::UUID(const std::string &str, bool dashes) noexcept
    {
        auto copy = str;
        if (dashes)
            copy.erase(std::remove(copy.begin(), copy.end(), '-'), copy.end());

        U64 tmp = 0;
        std::from_chars(copy.data(), copy.data() + 8, this->msb, 16);
        this->msb <<= 16;
        std::from_chars(copy.data() + 8, copy.data() + 8 + 4, tmp, 16);
        this->msb |= tmp;
        this->msb <<= 16;
        std::from_chars(copy.data() + 8 + 4, copy.data() + 8 + 4 + 4, tmp, 16);
        this->msb |= tmp;

        std::from_chars(copy.data() + 8 + 4 + 4, copy.data() + 8 + 4 + 4 + 4, this->lsb, 16);
        this->lsb <<= 48;
        std::from_chars(copy.data() + 8 + 4 + 4 + 4, copy.data() + 8 + 4 + 4 + 4 + 12, tmp, 16);
        this->lsb |= tmp;
    }

    UUID UUID::random()
    {
        Byte randomBytes[16];
        COMiC::Crypto::secureBytes(randomBytes, 16);

        randomBytes[6] &= 0x0F;
        randomBytes[6] |= 0x40;
        randomBytes[8] &= 0x3F;
        randomBytes[8] |= 0x80;

        return UUID(randomBytes);
    }

    UUID UUID::fromName(const char *name) noexcept
    {
        Byte md5Bytes[16];
        Crypto::MD5::hash(name, md5Bytes);

        md5Bytes[6] &= 0x0F;
        md5Bytes[6] |= 0x30;
        md5Bytes[8] &= 0x3F;
        md5Bytes[8] |= 0x80;

        return UUID(md5Bytes);
    }

    constexpr U32 UUID::version() const noexcept
    {
        return (U32) (this->msb >> 12) & 0x0F;
    }

    constexpr U32 UUID::variant() const noexcept
    {
        return (U32) (((U64) this->lsb >> (64 - ((U64) this->lsb >> 62))) & (this->lsb >> 63));
    }

    constexpr U64 UUID::timestamp() const noexcept
    {
        if (this->version() != 1)
            return -1;

        return (this->msb & 0x0FFFLL) << 48 | ((this->msb >> 16) & 0x0FFFFLL) << 32 | (U64) ((U64) this->msb >> 32);
    }

    constexpr U32 UUID::clockSequence() const noexcept
    {
        if (this->version() != 1)
            return -1;

        return (U32) ((U64) (this->lsb & 0x3FFF000000000000LL) >> 48);
    }

    constexpr U64 UUID::node() const noexcept
    {
        if (this->version() != 1)
            return -1;

        return this->lsb & 0x0000FFFFFFFFFFFFLL;
    }

    std::string digits(U64 val, USize digits)
    {
        U64 hi = 1ULL << (digits * 4);
        std::stringstream stream;
        stream << std::hex << (hi | (val & (hi - 1)));

        return {stream.str().substr(1)};
    }

    std::string UUID::toString() const noexcept
    {
        return (digits(this->msb >> 32, 8) + "-" +
                digits(this->msb >> 16, 4) + "-" +
                digits(this->msb, 4) + "-" +
                digits(this->lsb >> 48, 4) + "-" +
                digits(this->lsb, 12));
    }
}
