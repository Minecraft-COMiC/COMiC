#include <windows.h>

#include <COMiC/types.h>
#include <COMiC/os.h>

COMiC_IfError COMiC_OS_Heap_Init(
        COMiC_Out COMiC_OS_Heap *self,
        COMiC_In COMiC_Optional(0) COMiC_USize max_size,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
)
{
    _COMiC_Unlock(HANDLE, self->heap_handle) = HeapCreate(0, 0, max_size);
    return (self->heap_handle == NULL) ? COMiC_ERROR : COMiC_SUCCESS;
}