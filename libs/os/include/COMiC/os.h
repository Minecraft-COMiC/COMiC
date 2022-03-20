#ifndef COMiC_OS_H
# define COMiC_OS_H

#include <COMiC/types.h>
#include <COMiC/error.h>


# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_OS_Heap COMiC_OS_Heap;

COMiC_Constructor
COMiC_IfError COMiC_OS_Heap_Init(
        COMiC_Out COMiC_OS_Heap *self,
        COMiC_In COMiC_Optional(0) COMiC_USize max_size,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
);


COMiC_Destructor
COMiC_IfError COMiC_OS_Heap_Finalize(
        COMiC_Out COMiC_OS_Heap *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
);

void *COMiC_MAlloc(
        COMiC_In COMiC_OS_Heap *self,
        COMiC_In COMiC_USize size
);

void *COMiC_ReAlloc(
        COMiC_In COMiC_OS_Heap *self,
        COMiC_In void *pointer,
        COMiC_In COMiC_USize size
);

void COMiC_Free(
        COMiC_In COMiC_OS_Heap *self,
        COMiC_In void *pointer
);

# ifdef __cplusplus
};
# endif


# include <COMiC/_os.h>

#endif /* COMiC_OS_H */
