#include "network_windows.h"

void network_init(ServerNetInfo *info)
{
    printf("---Starting COMiC server v%s on Windows---\n", COMiC_VERSION);

    WSADATA wsa;
    printf("Initializing WinSock... ");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed! Error code : %d", WSAGetLastError());
        exit(1);
    }
    puts("Done");

    printf("Creating socket... ");
    if ((info->socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
        exit(1);
    }
    puts("Done");

    // Prepare the server structure:
    info->address.sin_family = AF_INET;
    InetPton(AF_INET, DEFAULT_SERVER_IP, &info->address.sin_addr.s_addr);
    info->address.sin_port = htons(DEFAULT_SERVER_PORT);

    printf("Binding... ");
    if (bind(info->socket, (struct sockaddr *) &info->address, sizeof(info->address)) ==
        SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(1);
    }
    puts("Done");
}

void network_listen_to_connections(
        ServerNetInfo server,
        ClientNetInfo *client,
        void (*onPacketReceive)(ClientNetInfo *, ByteBuffer *)
)
{
    // Listen to incoming connections:
    listen(server.socket, 3);

    puts("Waiting for incoming connections...");

    // Accept incoming connection:
    int c = sizeof(client->address);
    client->socket = (int) accept(server.socket, (struct sockaddr *) &client->address, &c);
    if (client->socket == INVALID_SOCKET)
    {
        printf("Accept failed with error code : %d", WSAGetLastError());
        exit(1);
    }

    puts("Connection accepted");

    int message_length;
    char bytes[512];
    while (420 > 69)
    {
        message_length = recv(client->socket, bytes, 512, 0);

        if (message_length > 0)
        {
            ByteBuffer buf = {.bytes = bytes, .index = 0, .size = network_buffer_read_var_int(&buf)};

            onPacketReceive(client, &buf);
        }
        else if (message_length == 0)
        {
            puts("Connection closed");
            break;
        }
        else
        {
            printf("Failed receiving data from client! Error code: %d\n", WSAGetLastError());
            break;
        }
    }
}

void network_send_packet(ClientNetInfo *connection, ByteBuffer *buf)
{
    network_buffer_prepare(buf);
    send(connection->socket, buf->bytes, (int) buf->size, 0);
    free(buf);
}

void network_cleanup(ServerNetInfo info)
{
    closesocket(info.socket);
    WSACleanup();
}