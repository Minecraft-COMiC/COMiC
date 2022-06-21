#ifndef COMiC_Core_OS_H
# define COMiC_Core_OS_H

#include "types.h"
#include "error.h"


# ifdef __cplusplus
extern "C" {
# endif

COMiC_IfError COMiC_NativeAllocPage(
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(NULL) char *error_message,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
);

COMiC_IfError COMiC_NativeDeallocPage(
        COMiC_Out COMiC_Error *error,
        COMiC_In void **pointer
);

# ifdef __cplusplus
};
# endif

# include <COMiC/core/_os.h>

#endif /* COMiC_Core_OS_H */
