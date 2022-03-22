#include <COMiC.h>

COMiC_Constructor
COMiC_IfError COMiC_Application_Init(
        COMiC_Out COMiC_Application *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_heap_size
)
{
    if (COMiC_Heap_Init(
            &(self->heap),
            error,
            max_heap_size
    ))
    {
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_Application_Finalize(
        COMiC_In COMiC_Application *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
)
{
    if (COMiC_Heap_Finalize(
            &(self->heap),
            error
    ))
    {
        return COMiC_ERROR;
    }
    return COMiC_SUCCESS;
}