#include <windows.h>

#include <COMiC/core.h>

#include "error.h"


COMiC_IfError COMiC_OS_Lock_Constructor(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    COMiC_UnusedArg(error);

    InitializeCriticalSection(&(self->critical_section));

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_OS_Lock_Destructor(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    COMiC_UnusedArg(error);

    DeleteCriticalSection(&(self->critical_section));

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_OS_Lock_Acquire(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    COMiC_UnusedArg(error);

    DeleteCriticalSection(&(self->critical_section));

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_OS_Lock_TryAcquire(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error,
        COMiC_Out COMiC_Bool *is_locked
) noexcept {
    COMiC_UnusedArg(error);

    *is_locked = TryEnterCriticalSection(&(self->critical_section));

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_OS_Lock_Release(
        COMiC_In COMiC_OS_Lock *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    COMiC_UnusedArg(error);

    LeaveCriticalSection(&(self->critical_section));

    return COMiC_SUCCESS;
}