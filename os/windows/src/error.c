#include <string.h>

#include <windows.h>

#include <COMiC/core.h>

#include "error.h"

static COMiC_IfError COMiC_OS_Windows_ErrorDestructor(
        COMiC_In COMiC_Error *self,
        COMiC_Out COMiC_Error *error
)
{
    return COMiC_ErrorHeap_DeAlloc(error, self->message);
}

static const COMiC_ErrorClass _COMiC_OS_Windows_SystemErrorClass = {
        .name = "Windows system error",
        .base_classes = NULL, // todo
        .extra_info_size = sizeof(COMiC_OS_Windows_ErrorExtraInfo),
        .destructor = COMiC_OS_Windows_ErrorDestructor
};

COMiC_ErrorClass const *const COMiC_OS_Windows_SystemErrorClass = &_COMiC_OS_Windows_SystemErrorClass;

COMiC_IfError COMiC_OS_Windows_FormatErrorMessage(
        COMiC_Out COMiC_Error *self
)
{
    char *system_allocated_message;
    DWORD error_code = GetLastError();
    FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error_code,
            0x409, /* en-US */
            (LPSTR) &system_allocated_message,
            0, NULL
    );

    COMiC_USize message_size = strlen(system_allocated_message) + 1;
    if (COMiC_ErrorHeap_Alloc(self, (void **) &(self->message), message_size + sizeof(COMiC_OS_Windows_ErrorExtraInfo)))
    { return COMiC_ERROR; }

    self->cls = COMiC_OS_Windows_SystemErrorClass;
    self->extra_data = (COMiC_OS_Windows_ErrorExtraInfo *) (((COMiC_UIntPtr) (self->message)) + sizeof(char) * message_size);
    ((COMiC_OS_Windows_ErrorExtraInfo *) (self->extra_data))->error_code = error_code;

    memcpy(self->message, system_allocated_message, sizeof(char) * message_size);
    LocalFree(system_allocated_message); // todo error handling

    return COMiC_ERROR;
}
