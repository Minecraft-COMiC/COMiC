#ifndef COMIC_NETWORK_WINDOWS_H
#define COMIC_NETWORK_WINDOWS_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include "../../network.h"
#include "../../network_constants.h"

struct COMiC_Network_ServerNetInfo
{
    struct sockaddr_in address;
    SOCKET socket;
};

struct COMiC_Network_ClientNetInfo
{
    struct sockaddr_in address;
    SOCKET socket;
    COMiC_Network_NetworkState state;
    char *username;
    COMiC_Util_JUUID uuid;
};

void COMiC_Network_init(COMiC_Network_ServerNetInfo *server);

void COMiC_Network_listen_to_connections(COMiC_Network_ServerNetInfo server, COMiC_Network_ClientNetInfo *client, void (*onPacketReceive)(COMiC_Network_ClientNetInfo *, COMiC_Network_ByteBuffer *));

void COMiC_Network_send_packet(COMiC_Network_ClientNetInfo *connection, COMiC_Network_ByteBuffer *buf);

void COMiC_Network_cleanup(COMiC_Network_ServerNetInfo server);

#endif //COMIC_NETWORK_WINDOWS_H
