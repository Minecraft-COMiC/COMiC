#include <COMiC/core.h>

COMiC_Constructor
COMiC_IfError COMiC_Application_Init(
        COMiC_Out COMiC_Application *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_heap_size
)
{
    if (COMiC_OS_Heap_Init(
            &_COMiC_Unlock(COMiC_OS_Heap *, self->heap),
            max_heap_size,
            error
    ))
    {
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_Application_Finalize(
        COMiC_Out COMiC_Application *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
)
{
    if (COMiC_OS_Heap_Finalize(
            &_COMiC_Unlock(COMiC_OS_Heap *, self->heap),
            error
    ))
    {
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}