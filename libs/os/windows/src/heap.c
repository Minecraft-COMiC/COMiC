#include <windows.h>

#include <COMiC/types.h>
#include <COMiC/os.h>

#include "error.h"

COMiC_Constructor
COMiC_IfError COMiC_OS_Heap_Init(
        COMiC_Out COMiC_OS_Heap *self,
        COMiC_In COMiC_Optional(0) COMiC_USize max_size,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
)
{
    _COMiC_Unlock(HANDLE *, self->heap_handle) = HeapCreate(0, 0, max_size);
    if (self->heap_handle == NULL)
    {
        if (error != NULL)
        {
            COMiC_OS_Windows_FormatErrorMessage(error, COMiC_ErrNo_FailedCreateHeap);
        }
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_OS_Heap_Finalize(
        COMiC_Out COMiC_OS_Heap *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
)
{
    if (HeapDestroy(_COMiC_Unlock(HANDLE *, self->heap_handle)) == 0)
    {
        if (error != NULL)
        {
            COMiC_OS_Windows_FormatErrorMessage(error, COMiC_ErrNo_FailedDestroyHeap);
        }
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}