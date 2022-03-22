#include <COMiC.h>

void COMiC_Error_NoDealloc(void *_)
{}

COMiC_Constructor
COMiC_IfError COMiC_Error_Set(
        COMiC_Out COMiC_Error *self,
        COMiC_In COMiC_ErrNo err_no,
        COMiC_In COMiC_Optional(NULL) char *message,
        COMiC_In COMiC_Optional(NULL) void *external_data,
        COMiC_In COMiC_Optional(COMiC_Error_NoDealloc) void (*free_func)(void *)
)
{
    self->err_no = err_no;
    self->message = message;
    self->external_data = external_data;
    self->free_func = free_func;
    return COMiC_ERROR;
}

COMiC_Constructor
void COMiC_Error_Init(
        COMiC_Out COMiC_Error *self
)
{
    self->err_no = COMiC_ErrNo_NoError;
    self->message = NULL;
    self->external_data = NULL;
    self->free_func = COMiC_Error_NoDealloc;
}


COMiC_Destructor
void COMiC_Error_Release(
        COMiC_In COMiC_Error *self
)
{
    if (self->message != NULL)
    {
        self->free_func(self->message);
    }
}
