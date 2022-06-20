#ifndef COMiC_OS_H
# define COMiC_OS_H

#include <COMiC/types.h>
#include <COMiC/error.h>


# ifdef __cplusplus
extern "C" {
# endif

COMiC_IfError _COMiC_NativeAlloc(
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
);


COMiC_IfError _COMiC_NativeReAlloc(
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_In COMiC_Out void **pointer,
        COMiC_In COMiC_USize new_size
);

COMiC_IfError _COMiC_NativeDealloc(
        COMiC_Out COMiC_Error *error,
        COMiC_In void **pointer
);

typedef struct COMiC_Thread COMiC_Thread;

COMiC_Destructor
COMiC_IfError COMiC_Thread_Release(
        COMiC_In COMiC_Thread *self,
        COMiC_Out COMiC_Error *error
);

# ifdef __cplusplus
};
# endif


# include <COMiC/_os.h>

#endif /* COMiC_OS_H */
