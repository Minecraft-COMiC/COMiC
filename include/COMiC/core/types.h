#ifndef COMiC_Core_TYPES_H
# define COMiC_Core_TYPES_H

# ifndef __cplusplus

#  include <stdint.h>
#  include <limits.h>

# else
#  include <cstdint>
#  include <climits>
# endif

# ifndef __cplusplus
#  define constexpr
#  define noexcept
# endif

# ifndef __cplusplus
#  ifndef NULL
#   define NULL ((void *)0)
#  endif
# endif

# define COMiC_BYTE_SIZE (CHAR_BIT)

typedef size_t COMiC_USize;
typedef uintptr_t COMiC_UIntPtr;
typedef unsigned char COMiC_Byte;
typedef unsigned int COMiC_FastestNumber;

typedef COMiC_FastestNumber COMiC_Bool;
# define COMiC_FALSE (0)
# define COMiC_TRUE (~0)

typedef COMiC_Bool COMiC_IfError;
# define COMiC_SUCCESS COMiC_FALSE
# define COMiC_ERROR COMiC_TRUE

typedef enum COMiC_ComparisonResult
{
    COMiC_LESS = -1,
    COMiC_EQUALS = 0,
    COMiC_GREATER = 1,
} COMiC_ComparisonResult;

# define COMiC_Optional(VALUE)
# define COMiC_UnusedArg(ARGUMENT) ((void)(ARGUMENT))

# define COMiC_In
# define COMiC_InOut
# define COMiC_Out

# define COMiC_ShiftPointer(POINTER, OFFSET) ((void *)(((COMiC_UIntPtr)(POINTER)) + ((COMiC_USize)(OFFSET))))
# define COMiC_FromSuperStruct(POINTER, STRUCT, FIELD) ((STRUCT *)(COMiC_ShiftPointer((POINTER), -offsetof(STRUCT, FIELD))))

#endif /* COMiC_Core_TYPES_H */
