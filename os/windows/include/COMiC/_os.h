#ifndef COMiC__OS_WINDOWS_H
# define COMiC__OS_WINDOWS_H

#include <windows.h>

struct COMiC_OS_Heap
{
    HANDLE heap_handle;
};

struct COMiC_OS_Socket
{
    SOCKET socket;
};

struct COMiC_OS_InternetAddress
{
    struct sockaddr_in address;
};

#endif /* COMiC__OS_WINDOWS_H */
