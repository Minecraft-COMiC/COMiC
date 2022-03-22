#ifndef COMiC_OS_H
# define COMiC_OS_H

#include <COMiC/types.h>
#include <COMiC/error.h>


# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_Heap COMiC_Heap;

COMiC_Constructor
COMiC_IfError COMiC_Heap_Init(
        COMiC_Out COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_size
);


COMiC_Destructor
COMiC_IfError COMiC_Heap_Finalize(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error
);

COMiC_IfError COMiC_Alloc(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
);

COMiC_IfError COMiC_ReAlloc(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_In COMiC_Out void **pointer,
        COMiC_In COMiC_USize new_size
);

COMiC_IfError COMiC_Free(
        COMiC_In COMiC_Heap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **pointer
);

typedef struct COMiC_Thread COMiC_Thread;

COMiC_Constructor
COMiC_IfError COMiC_Thread_WrapCurrent(
        COMiC_Out COMiC_Thread *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Application *owner
);

COMiC_Destructor
COMiC_IfError COMiC_Thread_Release(
        COMiC_In COMiC_Thread *self,
        COMiC_Out COMiC_Error *error
);

typedef struct COMiC_Socket COMiC_Socket;
typedef struct COMiC_InternetAddress COMiC_InternetAddress;

# ifdef __cplusplus
};
# endif


# include <COMiC/_os.h>

#endif /* COMiC_OS_H */
