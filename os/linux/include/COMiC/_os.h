#ifndef COMiC__OS_LINUX_H
#define COMiC__OS_LINUX_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

struct COMiC_Network_Socket {
    int socket;
};

struct COMiC_OS_InternetAddress
{
    struct sockaddr_in address;
};

#endif // COMiC__OS_LINUX_H