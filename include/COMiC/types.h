#ifndef COMiC_TYPES_H
# define COMiC_TYPES_H

# ifndef __cplusplus

#  include <stdint.h>

# else
#  include <cstdint>
# endif

# ifndef __cplusplus
#  ifndef NULL
#   define NULL ((void *)0)
#  endif
# endif

typedef int COMiC_Bool;
# define COMiC_FALSE (0)
# define COMiC_TRUE (1)

typedef COMiC_Bool COMiC_IfError;
# define COMiC_SUCCESS COMiC_FALSE
# define COMiC_ERROR COMiC_TRUE

typedef size_t COMiC_USize;
typedef uintptr_t COMiC_UIntPtr;
typedef int_fast8_t COMiC_Int8;
typedef uint_fast8_t COMiC_UInt8;
typedef int_fast16_t COMiC_Int16;
typedef uint_fast16_t COMiC_UInt16;
typedef int_fast32_t COMiC_Int32;
typedef uint_fast32_t COMiC_UInt32;
typedef int_fast64_t COMiC_Int64;
typedef uint_fast64_t COMiC_UInt64;
typedef uint8_t COMiC_Byte;
typedef uint16_t COMiC_Word;
typedef uint32_t COMiC_DWord;
typedef uint64_t COMiC_QWord;

# define COMiC_Constructor
# define COMiC_Destructor

# define COMiC_Optional(VALUE)

# define COMiC_In
# define COMiC_Out

#endif /* COMiC_TYPES_H */
