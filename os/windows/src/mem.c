#include <windows.h>

#include <COMiC/core.h>

#include "error.h"

COMiC_IfError COMiC_NativeAllocOnPages(
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
) noexcept {
    *pointer = VirtualAlloc(
            NULL,
            (SIZE_T) size,
            MEM_COMMIT,
            PAGE_EXECUTE_READWRITE
    );
    if (*pointer == NULL)
    { return COMiC_OS_Windows_FormatErrorMessage(error); }
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_NativeDeAllocOnPages(
        COMiC_Out COMiC_Error *error,
        COMiC_In void *pointer
) noexcept {
    if (VirtualFree(
            pointer,
            0,
            MEM_RELEASE
    ) == 0)
    { return COMiC_OS_Windows_FormatErrorMessage(error); }
    return COMiC_SUCCESS;
}

COMiC_USize COMiC_OS_GetNativeMemoryPageSize(void) noexcept {
    SYSTEM_INFO data;

    GetSystemInfo(&data);

    return (COMiC_USize) (data.dwPageSize);
}
