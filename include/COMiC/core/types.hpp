#ifndef COMiC_Core_TYPES_HPP
# define COMiC_Core_TYPES_HPP

# include <cstddef>
# include <cstdint>
# include <climits>
//# include <new>

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
    using FastestSInt = signed int;
    using FastestUInt = unsigned int;
    using Bool = bool;

    static const constexpr inline COMiC::USize BYTE_SIZE = CHAR_BIT;

#   ifndef __has_cpp_attribute
#    define __has_cpp_attribute(ATTR_NAME) (0)
#   endif

#   if __has_cpp_attribute(deprecated)
#    define COMiC_DEPRECATED(WHY) [[deprecated( WHY )]]
#   else
#    define COMiC_DEPRECATED(WHY)
#   endif

#   if __has_cpp_attribute(noreturn)
#    define COMiC_NORETURN [[noreturn]]
#   else
#    define COMiC_NORETURN
#   endif

#   if __has_cpp_attribute(nodiscard)
#    define COMiC_NODISCARD [[nodiscard]]
#   elif __has_cpp_attribute(gnu::warn_unused_result)
#    define COMiC_NODISCARD [[gnu::warn_unused_result]]
#   else
#    define COMiC_NODISCARD
#   endif

#   if __has_cpp_attribute(maybe_unused)
#    define COMiC_UNUSED [[maybe_unused]]
#   elif __has_cpp_attribute(gnu::unused)
#    define COMiC_UNUSED [[gnu::unused]]
#   else
#    define COMiC_UNUSED
#   endif

    enum COMiC_NODISCARD IfError
    {
        SUCCESS = false,
        FAIL    = true
    };

    enum COMiC_NODISCARD IteratorState
    {
        VALUE_FOUND = true,
        ENDED       = false
    };

    enum COMiC_NODISCARD ComparisonResult
    {
        LESS    = -1,
        EQUAL   = 0,
        GREATER = 1,
    };

#   define COMiC_In
#   define COMiC_InOut
#   define COMiC_Out


    template<typename in_t, typename out_t>
    static constexpr inline out_t *shiftPointerL(
            COMiC_In in_t *pointer,
            COMiC_In COMiC::USize offset
    ) noexcept
    { return (out_t *) (((COMiC::UIntPtr) pointer) - offset); }

    template<typename in_t, typename out_t>
    static constexpr inline out_t *shiftPointerR(
            COMiC_In in_t *pointer,
            COMiC_In COMiC::USize offset
    ) noexcept
    { return (out_t *) (((COMiC::UIntPtr) pointer) + offset); }

#   define COMiC_PointerOnly

    enum ByteOrder
    {
        /* Don't use this values, they're just for fun */
        BIG_ENDIAN    = 0xFEFF,
        LITTLE_ENDIAN = 0xFFFE
    };

    static constexpr inline COMiC::ByteOrder getSystemByteOrder() noexcept
    {
        COMiC::U8 marker = 0x0001;
        return (*(COMiC::Byte *) &marker == 0) ? COMiC::ByteOrder::BIG_ENDIAN : COMiC::ByteOrder::LITTLE_ENDIAN;
    }
}

template<typename obj_t>
void *operator new(COMiC::USize size, obj_t *where)
{
    return where;
}

#endif /* COMiC_Core_TYPES_HPP */
