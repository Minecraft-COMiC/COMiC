#include <windows.h>

#include <COMiC/types.h>
#include <COMiC/error.h>

#include "error.h"

static void LocalFree_NoReturn(void *p)
{
    LocalFree(p);
}

COMiC_Constructor
void COMiC_OS_Windows_FormatErrorMessage(
        COMiC_Out COMiC_Error *self,
        COMiC_In COMiC_ErrNo err_no
)
{
    FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            0x409, /* en-US */
            (LPSTR) &(self->message),
            0, NULL
    );
    _COMiC_Unlock(void (**)(void *), self->free_func) = LocalFree_NoReturn;
    _COMiC_Unlock(COMiC_ErrNo *, self->err_no) = err_no;
    _COMiC_Unlock(void **, self->external_data) = NULL;
}
