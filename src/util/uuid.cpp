#include <charconv>
#include <sstream>
#include <algorithm>
#include <COMiC/util.hpp>
#include <COMiC/crypto.hpp>

namespace COMiC::Util
{
    UUID::UUID(const Byte data[16]) noexcept
    {
        U64 msb_ = 0, lsb_ = 0;

        for (I32 i = 0; i < 8; i++)
            msb_ = (msb_ << 8) | (data[i] & 0xFF);

        for (I32 i = 8; i < 16; i++)
            lsb_ = (lsb_ << 8) | (data[i] & 0xFF);

        this->msb = msb_;
        this->lsb = lsb_;
    }

    UUID::UUID(const std::string &str) noexcept
    {
        auto copy = str;
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
        ByteVector randomBytes(16);
        Crypto::secureBytes(randomBytes);

        randomBytes[6] &= 0x0F;
        randomBytes[6] |= 0x40;
        randomBytes[8] &= 0x3F;
        randomBytes[8] |= 0x80;

        return UUID(randomBytes.data());
    }

    UUID UUID::fromName(const std::string &name) noexcept
    {
        auto md5Bytes = Crypto::MD5::hash(name);

        md5Bytes[6] &= 0x0F;
        md5Bytes[6] |= 0x30;
        md5Bytes[8] &= 0x3F;
        md5Bytes[8] |= 0x80;

        return UUID(md5Bytes.data());
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
