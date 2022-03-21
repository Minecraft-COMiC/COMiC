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

COMiC_IfError COMiC_Alloc(
        COMiC_In COMiC_OS_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
);

COMiC_IfError COMiC_ReAlloc(
        COMiC_In COMiC_OS_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Out void **pointer,
        COMiC_In COMiC_USize new_size
);

COMiC_IfError COMiC_Free(
        COMiC_In COMiC_OS_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **pointer
);

typedef struct COMiC_OS_Socket COMiC_OS_Socket;
typedef struct COMiC_OS_InternetAddress COMiC_OS_InternetAddress;

# ifdef __cplusplus
};
# endif


# include <COMiC/_os.h>

#endif /* COMiC_OS_H */
