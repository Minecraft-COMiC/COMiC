#ifndef COMiC_Core_OS_H
# define COMiC_Core_OS_H

# include "types.h"
# include "error.h"


# ifdef __cplusplus
extern "C" {
# endif

COMiC_IfError COMiC_NativeAllocOnPages(
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
) noexcept;

COMiC_IfError COMiC_NativeDeAllocOnPages(
        COMiC_Out COMiC_Error *error,
        COMiC_In void *pointer
) noexcept;

COMiC_USize COMiC_OS_GetNativeMemoryPageSize(void) noexcept;

static inline COMiC_USize COMiC_OS_GetNativeMemoryPageCount(
        COMiC_In COMiC_USize size
) noexcept
{ return (size + COMiC_OS_GetNativeMemoryPageSize() - 1) / COMiC_OS_GetNativeMemoryPageSize(); }

typedef struct COMiC_OS_Lock COMiC_OS_Lock;

COMiC_IfError COMiC_OS_Lock_Constructor(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_OS_Lock_Destructor(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_OS_Lock_Acquire(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_OS_Lock_TryAcquire(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error,
        COMiC_Out COMiC_Bool *is_locked
) noexcept;

COMiC_IfError COMiC_OS_Lock_Release(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept;

# ifdef __cplusplus
};
# endif

# include <COMiC/core/_os.h>

#endif /* COMiC_Core_OS_H */
