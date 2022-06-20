#ifndef COMiC__OS_H
# define COMiC__OS_H

# include <windows.h>

# ifdef __cplusplus
extern "C" {
# endif

struct COMiC_Heap
{
    HANDLE heap_handle;
};

struct COMiC_Thread
{
    COMiC_BaseApplication *app;
    HANDLE thread_handle;
};

# ifdef __cplusplus
};
# endif

#endif /* COMiC__OS_H */
