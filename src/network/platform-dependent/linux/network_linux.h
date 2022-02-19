#ifndef COMIC_NETWORK_LINUX_H
#define COMIC_NETWORK_LINUX_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "../../network.h"
#include "../../network_constants.h"

struct ServerNetInfo
{
    struct sockaddr_in address;
    int socket;
};

struct ClientNetInfo
{
    struct sockaddr_in address;
    int socket;
    NetworkState state;
    char *username;
    JUUID uuid;
};

void network_init(ServerNetInfo *info);

void network_listen_to_connections(ServerNetInfo server, ClientNetInfo *client, void (*onPacketReceive)(ClientNetInfo *, ByteBuffer *));

void network_send_packet(ClientNetInfo *connection, ByteBuffer *buf);

void network_cleanup(ServerNetInfo info);

#endif //COMIC_NETWORK_LINUX_H
