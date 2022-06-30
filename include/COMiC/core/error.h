#ifndef COMiC_Core_ERROR_H
# define COMiC_Core_ERROR_H

# include "types.h"

# ifdef __cplusplus
extern "C" {
#endif

typedef enum COMiC_ErrNo
{
    COMiC_ErrNo_NoError = 0,
    COMiC_ErrNo_External = 1,
    COMiC_ErrNo_BoundLanguageException = 2,
    COMiC_ErrNo_FailedCreateHeap = 3,
    COMiC_ErrNo_FailedDestroyHeap = 4,
    COMiC_ErrNo_MemoryError = 5,
    COMiC_ErrNo_EmptyAlloc = 6,
    COMiC_ErrNo_FreeInvalidPointer = 7,
    COMiC_ErrNo_Overflow = 8,
} COMiC_ErrNo;

typedef struct COMiC_Error
{
    COMiC_ErrNo err_no;
    COMiC_Optional(NULL) char *message;
    COMiC_Optional(NULL) void *external_data;

    struct
    {
        COMiC_Optional(COMiC_FALSE) COMiC_Bool is_message_on_heap;
        COMiC_Optional(NULL)

        void (*external_data_finalizer)(void *);
    } dealloc_info;
} COMiC_Error;

COMiC_Constructor
static constexpr inline void COMiC_Error_Init(
        COMiC_Out COMiC_Error *self
) noexcept
{
    self->err_no = COMiC_ErrNo_NoError;
    self->message = NULL;
    self->external_data = NULL;
    self->dealloc_info.is_message_on_heap = COMiC_FALSE;
    self->dealloc_info.external_data_finalizer = NULL;
}

COMiC_Constructor
static constexpr inline COMiC_IfError COMiC_Error_Set(
        COMiC_Out COMiC_Error *self,
        COMiC_In COMiC_ErrNo err_no,
        COMiC_In COMiC_Optional(NULL) char *message,
        COMiC_In COMiC_Optional(NULL) void *external_data,
        COMiC_Optional(COMiC_FALSE) COMiC_Bool is_message_on_heap,
        COMiC_Optional(NULL) void (*external_data_finalizer)(void *)
) noexcept
{
    self->err_no = err_no;
    self->message = message;
    self->external_data = external_data;
    self->dealloc_info.is_message_on_heap = is_message_on_heap;
    self->dealloc_info.external_data_finalizer = external_data_finalizer;
    return COMiC_ERROR;
}

COMiC_Destructor
void COMiC_Error_Release(
        COMiC_In COMiC_Error *self
);

COMiC_Destructor COMiC_Constructor
static inline void COMiC_Error_Clear(
        COMiC_In COMiC_Error *self
) noexcept
{
    COMiC_Error_Release(self);
    COMiC_Error_Init(self);
}


# ifdef __cplusplus
};
#endif

#endif /* COMiC_Core_ERROR_H */