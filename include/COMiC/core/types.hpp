#ifndef COMiC_Core_TYPES_HPP
# define COMiC_Core_TYPES_HPP

#  include <cstddef>
#  include <cstdint>
#  include <climits>
#  include <vector>
#  include <array>
#  include <variant>

namespace COMiC
{
    using USize = size_t;
    using UIntPtr = uintptr_t;
    using I8 = int_least8_t;
    using U8 = uint_least8_t;
    using I16 = int_least16_t;
    using U16 = uint_least16_t;
    using I32 = int_least32_t;
    using U32 = uint_least32_t;
    using I64 = int_least64_t;
    using U64 = uint_least64_t;
    using Byte = unsigned char;

    class Error : public std::exception
    {
    protected:
        Error() : std::exception()
        {
        }

    public:
        [[nodiscard]] char const *what() const noexcept override
        {
            return "COMiC: Unknown error";
        }
    };

    using ByteVector = std::vector<Byte>;

    template<USize N>
    struct ByteArray : public std::array<Byte, N>
    {
        using std::array<Byte, N>::array;
    };

    static inline const USize BYTE_SIZE = CHAR_BIT;

    enum [[nodiscard]] IfError : bool
    {
        SUCCESS = false,
        FAIL = true
    };

    enum ComparisonResult
    {
        LESS = -1,
        EQUAL = 0,
        GREATER = 1,
    };
}
#endif /* COMiC_Core_TYPES_HPP */
