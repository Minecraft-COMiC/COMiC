#include <windows.h>

#include <COMiC.h>


COMiC_Constructor
COMiC_IfError COMiC_OS_Thread_WrapCurrent(
        COMiC_Out COMiC_OS_Thread *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Application *owner
)
{
    self->app = owner;
    self->thread_handle = GetCurrentThread();
    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_OS_Thread_Release(
        COMiC_Out COMiC_OS_Thread *self,
        COMiC_Out COMiC_Error *error
)
{
    return COMiC_SUCCESS;
}