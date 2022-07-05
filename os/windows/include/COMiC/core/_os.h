#ifndef COMiC_Core__OS_H
# define COMiC_Core__OS_H

# include <windows.h>

# ifdef __cplusplus
extern "C" {
# endif

struct COMiC_OS_Lock
{
    CRITICAL_SECTION critical_section;
};

typedef struct COMiC_OS_Windows_ErrorExtraInfo
{
    DWORD error_code;
} COMiC_OS_Windows_ErrorExtraInfo;

extern COMiC_ErrorClass const *const COMiC_OS_Windows_SystemErrorClass;


# ifdef __cplusplus
};
# endif

#endif /* COMiC_Core__OS_H */
