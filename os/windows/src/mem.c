#include <windows.h>

#include <COMiC.h>

#include "error.h"

COMiC_IfError COMiC_Alloc(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
)
{
    *pointer = HeapAlloc(
            self->heap_handle,
            0,
            size
    );

    if (*pointer == NULL)
    {
        COMiC_Error_Set(
                error,
                COMiC_ErrNo_MemoryError,
                error_message,
                NULL,
                COMiC_Error_NoDealloc
        );
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_ReAlloc(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_In COMiC_Out void **pointer,
        COMiC_In COMiC_USize new_size
)
{
    *pointer = HeapReAlloc(
            self->heap_handle,
            0,
            *pointer,
            new_size
    );


    if (*pointer == NULL)
    {
        COMiC_Error_Set(
                error,
                COMiC_ErrNo_MemoryError,
                error_message,
                NULL,
                COMiC_Error_NoDealloc
        );
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_Free(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **pointer
)
{
    if (HeapFree(
            self->heap_handle,
            0,
            *pointer
    ) == 0)
    {
        COMiC_OS_Windows_FormatErrorMessage(
                error,
                COMiC_ErrNo_MemoryError
        );
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}