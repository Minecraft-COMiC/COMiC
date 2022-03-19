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
    COMiC_ErrNo_FailedCreateHeap = 2,
    COMiC_ErrNo_FailedDestroyHeap = 3,
} COMiC_ErrNo;

void COMiC_Error_NoDealloc(void *);

typedef struct COMiC_Error
{
    const COMiC_ErrNo err_no;
    COMiC_Optional(NULL) const char *const message;
    COMiC_Optional(NULL) const void *const external_data;

    COMiC_Optional(COMiC_Error_NoDealloc) const void (*const free_func)(void *);
} COMiC_Error;

COMiC_Constructor
void COMiC_Error_Init(
        COMiC_Out COMiC_Error *self
);

COMiC_Constructor
void COMiC_Error_Set(
        COMiC_Out COMiC_Error *self,
        COMiC_ErrNo err_no,
        COMiC_Optional(NULL) const char *message,
        COMiC_Optional(NULL) const void *external_data,
        COMiC_Optional(COMiC_Error_NoDealloc) const void (*free_func)(void *)
);

COMiC_Destructor
void COMiC_Error_Release(
        COMiC_In COMiC_Error *self
);

COMiC_Destructor COMiC_Constructor
static inline void COMiC_Error_Clear(
        COMiC_Out COMiC_Error *self
)
{
    COMiC_Error_Release(self);
    COMiC_Error_Init(self);
}


# ifdef __cplusplus
};
#endif

#endif /* COMiC_ERROR_H */
