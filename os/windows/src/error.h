#ifndef COMiC__OS_WINDOWS_ERROR_H
# define COMiC__OS_WINDOWS_ERROR_H

# include <COMiC/core/types.hpp>
# include <COMiC/core/error.h>

# ifdef __cplusplus
extern "C" {
# endif

COMiC_IfError COMiC_OS_Windows_FormatErrorMessage(
        COMiC_Out COMiC_Error *self
);

# ifdef __cplusplus
};
# endif
#endif /* COMiC__OS_WINDOWS_ERROR_H */
