#include <stdio.h>
#include <stdarg.h>

//#include <COMiC/core.h>

void COMiC_Error_EmptyConstructor(
        COMiC_Out COMiC_Error *self
) noexcept {
    self->cls = NULL;
    self->extra_data = NULL;
    self->message = NULL;
}

COMiC_IfError COMiC_Error_Clear(
        COMiC_InOut COMiC_Error *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    if (COMiC_Error_Destructor(self, error))
    { return COMiC_ERROR; }
    COMiC_Error_EmptyConstructor(self);
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_Error_Destructor(
        COMiC_InOut COMiC_Error *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    if (self->cls != NULL)
    { return self->cls->destructor(self, error); }
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_Error_FormatMessage(
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize extra_data_size,
        COMiC_Out char **formatted_string,
        COMiC_Out COMiC_Optional(NULL) void **extra_data,
        COMiC_In char const *format,
        COMiC_In ...
) noexcept {
    va_list varargs;
            va_start(varargs, format);
    int message_size_i = vsnprintf(NULL, 0, format, varargs);
            va_end(varargs);

    if (message_size_i < 0)
    {
        // todo
        return COMiC_ERROR;
    }
    COMiC_USize message_size = ((COMiC_USize) message_size_i) + 1;

    if (COMiC_ErrorHeap_Alloc(
            error,
            (void **) formatted_string,
            message_size + extra_data_size
    ))
    { return COMiC_ERROR; }

    if (extra_data != NULL)
    { *extra_data = (void *) (((COMiC_USize) formatted_string) + message_size); }

            va_start(varargs, format);
    if (vsnprintf(*formatted_string, message_size, format, varargs) < 0)
    {
                va_end(varargs);

        // todo
        return COMiC_ERROR;
    }
            va_end(varargs);

    return COMiC_SUCCESS;
}