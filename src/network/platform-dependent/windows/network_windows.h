#ifndef COMIC_NETWORK_WINDOWS_H
#define COMIC_NETWORK_WINDOWS_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include "../../network.h"
#include "../../network_constants.h"

struct ServerNetInfo
{
    struct sockaddr_in address;
    SOCKET socket;
};

struct ClientNetInfo
{
    struct sockaddr_in address;
    SOCKET socket;
    NetworkState state;
    char *username;
    JUUID uuid;
};

void network_init(ServerNetInfo *info);

void network_listen_to_connections(ServerNetInfo server, ClientNetInfo *client, void (*onPacketReceive)(ClientNetInfo *, ByteBuffer *));

void network_send_packet(ClientNetInfo *connection, ByteBuffer *buf);

void network_cleanup(ServerNetInfo info);

#endif //COMIC_NETWORK_WINDOWS_H
