#ifndef COMiC_Core_ERROR_H
# define COMiC_Core_ERROR_H

# include "types.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_Error COMiC_Error;

typedef COMiC_IfError (*const COMiC_ErrorClass_DestructorFunc)(COMiC_Error *self, COMiC_Error *error);

typedef struct COMiC_ErrorClass
{
    struct COMiC_ErrorClass const *const base_classes;
    const COMiC_USize extra_info_size;
    const COMiC_ErrorClass_DestructorFunc destructor;
} COMiC_ErrorClass;

struct COMiC_Error
{
    COMiC_ErrorClass const *cls;
    char const *message;
    void const *extra_data;
};

COMiC_IfError COMiC_ErrorHeap_Alloc(
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
) noexcept;

COMiC_IfError COMiC_ErrorHeap_DeAlloc(
        COMiC_Out COMiC_Error *error,
        COMiC_In void *pointer
) noexcept;

COMiC_IfError COMiC_Error_FormatMessage(
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize extra_data_size,
        COMiC_Out char **formatted_string,
        COMiC_Out COMiC_Optional(NULL) void **extra_data,
        COMiC_In char const *format,
        COMiC_In ...
) noexcept;

void COMiC_Error_EmptyConstructor(
        COMiC_Out COMiC_Error *self
) noexcept;

COMiC_IfError COMiC_Error_Clear(
        COMiC_InOut COMiC_Error *self,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_Error_Destructor(
        COMiC_InOut COMiC_Error *self,
        COMiC_Out COMiC_Error *error
) noexcept;

static constexpr inline COMiC_Bool COMiC_Error_IsSet(
        COMiC_InOut COMiC_Error *self
) noexcept
{ return self->cls != NULL; }


# ifdef __cplusplus
};
#endif

#endif /* COMiC_Core_ERROR_H */
