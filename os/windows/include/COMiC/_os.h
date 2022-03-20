#ifndef COMiC__OS_H
# define COMiC__OS_H

# include <windows.h>

# ifdef __cplusplus
extern "C" {
# endif

struct COMiC_OS_Heap
{
    HANDLE heap_handle;
};

struct COMiC_OS_Thread
{
    COMiC_Application *app;
    HANDLE thread_handle;
};

# ifdef __cplusplus
};
# endif

#endif /* COMiC__OS_H */
