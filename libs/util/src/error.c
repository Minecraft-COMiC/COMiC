#include <COMiC/types.h>
#include <COMiC/error.h>

COMiC_Destructor
void COMiC_Error_Release(
        COMiC_In COMiC_Error *self
)
{
    if (self->message != NULL)
    {
        self->free_func(_COMiC_Unlock(char **, self->message));
    }
}

void COMiC_Error_NoDealloc(void *_)
{}


COMiC_Constructor
void COMiC_Error_Set(
        COMiC_Out COMiC_Error *self,
        COMiC_ErrNo err_no,
        COMiC_Optional(NULL) const char *message,
        COMiC_Optional(NULL) const void *external_data,
        COMiC_Optional(COMiC_Error_NoDealloc) const void (*free_func)(void *)
)
{
    _COMiC_Unlock(COMiC_ErrNo *, self->err_no) = err_no;
    _COMiC_Unlock(const char **, self->message) = message;
    _COMiC_Unlock(const void **, self->external_data) = external_data;
    _COMiC_Unlock(const void (**)(void *), self->err_no) = free_func;
}

COMiC_Constructor
void COMiC_Error_Init(
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *self
)
{
    _COMiC_Unlock(COMiC_ErrNo *, self->err_no) = COMiC_ErrNo_NoError;
    _COMiC_Unlock(const char **, self->message) = NULL;
    _COMiC_Unlock(const void **, self->external_data) = NULL;
    _COMiC_Unlock(const void (**)(void *), self->err_no) = COMiC_Error_NoDealloc;
}