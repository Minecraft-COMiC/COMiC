#include <COMiC/core.h>

void COMiC_Error_NoDealloc(void *_)
{}



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
