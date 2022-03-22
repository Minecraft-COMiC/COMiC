#ifndef COMiC_ERROR_H
# define COMiC_ERROR_H

# include <COMiC/types.h>

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

void COMiC_Error_NoDealloc(void *);

typedef struct COMiC_Error
{
    COMiC_ErrNo err_no;
    COMiC_Optional(NULL) char *message;
    COMiC_Optional(NULL) void *external_data;

    COMiC_Optional(COMiC_Error_NoDealloc) void (*free_func)(void *);
} COMiC_Error;

COMiC_Constructor
void COMiC_Error_Init(
        COMiC_Out COMiC_Error *self
);

COMiC_Constructor
COMiC_IfError COMiC_Error_Set(
        COMiC_Out COMiC_Error *self,
        COMiC_In COMiC_ErrNo err_no,
        COMiC_In COMiC_Optional(NULL) char *message,
        COMiC_In COMiC_Optional(NULL) void *external_data,
        COMiC_In COMiC_Optional(COMiC_Error_NoDealloc) void (*free_func)(void *)
);

COMiC_Destructor
void COMiC_Error_Release(
        COMiC_In COMiC_Error *self
);

COMiC_Destructor COMiC_Constructor
static inline void COMiC_Error_Clear(
        COMiC_In COMiC_Out COMiC_Error *self
)
{
    COMiC_Error_Release(self);
    COMiC_Error_Init(self);
}


# ifdef __cplusplus
};
#endif

#endif /* COMiC_ERROR_H */
