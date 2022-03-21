#include <windows.h>

#include <COMiC.h>

#include "error.h"

COMiC_Constructor
COMiC_IfError COMiC_Heap_Init(
        COMiC_Out COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_size
)
{
    self->heap_handle = HeapCreate(0, 0, max_size);
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
COMiC_IfError COMiC_Heap_Finalize(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error
)
{
    if (HeapDestroy(self->heap_handle) == 0)
    {
        if (error != NULL)
        {
            COMiC_OS_Windows_FormatErrorMessage(error, COMiC_ErrNo_FailedDestroyHeap);
        }
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}