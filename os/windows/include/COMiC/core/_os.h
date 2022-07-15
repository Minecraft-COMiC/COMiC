#ifndef COMiC_Core__OS_H
# define COMiC_Core__OS_H

#include <cstdio>
#include <windows.h>
#include <COMiC/core.hpp>
#include <COMiC/network.hpp>

namespace COMiC::Network::OS
{
    struct Socket
    {
        SOCKET socket;
    };

    struct InetAddr
    {
        struct sockaddr_in address;
    };
}

#endif /* COMiC_Core__OS_H */
