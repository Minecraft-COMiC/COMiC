#ifndef COMiC_Core_TYPES_HPP
# define COMiC_Core_TYPES_HPP

#  include <cstddef>
#  include <cstdint>
#  include <climits>

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

    static const constexpr COMiC::USize BYTE_SIZE = CHAR_BIT;

    enum IfError
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

    template<typename in_t, typename out_t>
    static constexpr inline out_t *shiftPointerL(in_t *pointer, COMiC::USize offset)
    {
        return (out_t *) (((COMiC::UIntPtr) pointer) - offset);
    }

    template<typename in_t, typename out_t>
    static constexpr inline out_t *shiftPointerR(in_t *pointer, COMiC::USize offset)
    {
        return (out_t *) (((COMiC::UIntPtr) pointer) + offset);
    }

}
#endif /* COMiC_Core_TYPES_HPP */
