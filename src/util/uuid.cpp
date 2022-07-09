#include <cstdlib>
#include <COMiC/util.hpp>
#include "COMiC/core/_os.h"

namespace COMiC::Util
{
    UUID UUID::random()
    {
        Byte randomBytes[16];
        COMiC::Core::OS::RandomBytes(randomBytes, 16);

        randomBytes[6] &= 0x0F;
        randomBytes[6] |= 0x40;
        randomBytes[8] &= 0x3F;
        randomBytes[8] |= -128;

        return UUID(randomBytes);
    }

    UUID UUID::fromName(const char *name) noexcept
    {
        Byte md5Bytes[16];
        MD5::hash(name, md5Bytes);

        md5Bytes[6] &= 0x0F;
        md5Bytes[6] |= 0x30;
        md5Bytes[8] &= 0x3F;
        md5Bytes[8] |= -128;

        return UUID(md5Bytes);
    }

    UUID UUID::fromString(const char *str) noexcept
    {
        const char *components[5];

        for (I8 i = 0, k = 0; str[i] != '\0'; i++)
            if (str[i] == '-')
                components[k++] = str + i;

        U64 most_sig_bits = strtoll(components[0], nullptr, 16);
        most_sig_bits <<= 16;
        most_sig_bits |= strtoll(components[1], nullptr, 16);
        most_sig_bits <<= 16;
        most_sig_bits |= strtoll(components[2], nullptr, 16);

        U64 least_sig_bits = strtoll(components[3], nullptr, 16);
        least_sig_bits <<= 48;
        least_sig_bits |= strtoll(components[4], nullptr, 16);

        return {most_sig_bits, least_sig_bits};
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

    void digits(U64 val, I32 digits, char *out)
    {
        U64 hi = 1LLU << (digits * 4);

        sprintf(out, "%llx", hi | (val & (hi - 1)));
    }

    void UUID::toString(char out[37]) const noexcept
    {
        char buf[13];

        digits(this->msb >> 32, 8, buf);
        memcpy(out + 0, buf, 8);
        out[8] = '-';

        digits(this->msb >> 16, 4, buf);
        memcpy(out + 9, buf, 4);
        out[13] = '-';

        digits(this->msb, 4, buf);
        memcpy(out + 14, buf, 4);
        out[18] = '-';

        digits(this->lsb >> 48, 4, buf);
        memcpy(out + 19, buf, 4);
        out[23] = '-';

        digits(this->lsb, 12, buf);
        memcpy(out + 24, buf, 12);
        out[36] = 0;
    }
}
