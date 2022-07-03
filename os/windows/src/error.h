#ifndef COMiC__OS_WINDOWS_ERROR_H
# define COMiC__OS_WINDOWS_ERROR_H

# include <COMiC/core/types.h>
# include <COMiC/core/error.h>

# ifdef __cplusplus
extern "C" {
# endif


void COMiC_OS_Windows_FormatErrorMessage(
        COMiC_Out COMiC_Error *self,
//        COMiC_In COMiC_ErrNo err_no
);

# ifdef __cplusplus
};
# endif
#endif /* COMiC__OS_WINDOWS_ERROR_H */
