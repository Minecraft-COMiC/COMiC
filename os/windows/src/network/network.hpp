#ifndef COMIC_OS_WINDOWS_NETWORK_H
#define COMIC_OS_WINDOWS_NETWORK_H

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <wininet.h>
#include <MSWSock.h>
#include <processthreadsapi.h>
#include <COMiC/network.hpp>

namespace COMiC::Network::OS
{
    enum IOCPOperation
    {
        INVALID = 0,
        SEND,
        RECEIVE,
        ACCEPT
    };

    struct ClientExtraData
    {
        WSAOVERLAPPED overlapped;
        SOCKET socket; // Client socket
        IOCPOperation operation;
        WSABUF buffer; // Buffer for send() or recv() operations, reference to `data` field
        ByteVector bytes; // Actual buffer for bytes
        DWORD processed;
        ClientNetInfo *connection;
    };
}

#endif // COMIC_OS_WINDOWS_NETWORK_H
