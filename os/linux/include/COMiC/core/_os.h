#ifndef COMiC__OS_LINUX_H
#define COMiC__OS_LINUX_H

#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/random.h>

namespace COMiC::Network::OS
{
    struct Socket {
        int socket;
    };

    struct InetAddr
    {
        struct sockaddr_in address;
    };
}

#endif // COMiC__OS_LINUX_H