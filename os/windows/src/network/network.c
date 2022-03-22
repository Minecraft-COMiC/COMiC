#include "network.h"

void COMiC_Network_Init(
        COMiC_Out COMiC_Network_ServerNetInfo *server
)
{
    WSADATA wsa;
    printf("Initializing WinSock... ");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed! Error code : %d", WSAGetLastError());
        exit(1);
    }
    puts("Done");

    printf("Creating socket... ");
    if ((server->socket.socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
        exit(1);
    }
    puts("Done");

    // Prepare the server structure:
    server->address.address.sin_family = AF_INET;
    InetPton(AF_INET, DEFAULT_SERVER_IP, &server->address.address.sin_addr.s_addr);
    server->address.address.sin_port = htons(DEFAULT_SERVER_PORT);

    printf("Binding... ");
    if (bind(server->socket.socket, (struct sockaddr *) &server->address, sizeof(server->address)) ==
        SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(1);
    }
    puts("Done");
}

void COMiC_Network_ListenToConnections(
        COMiC_In COMiC_Network_ServerNetInfo server,
        COMiC_In COMiC_Network_ClientNetInfo *client,
        COMiC_In void (*onPacketReceive)(COMiC_Network_ClientNetInfo *, COMiC_Network_Buffer *)
)
{
    // Listen to incoming connections:
    listen(server.socket.socket, 3);

    puts("Waiting for incoming connections...");

    // Accept incoming connection:
    int c = sizeof(client->address);
    client->socket.socket = (int) accept(server.socket.socket, (struct sockaddr *) &client->address, &c);
    if (client->socket.socket == INVALID_SOCKET)
    {
        printf("Accept failed with error code : %d", WSAGetLastError());
        exit(1);
    }

    puts("Connection accepted");

    int message_length;
    char bytes[512];
    while (420 > 69)
    {
        message_length = recv(client->socket.socket, bytes, 512, 0);

        if (message_length > 0)
        {
            COMiC_Network_Buffer buf = {.bytes = bytes, .index = 0, .size = COMiC_Network_Buffer_ReadVarInt(&buf)};

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

void COMiC_Network_SendPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_Buffer *buf
)
{
    COMiC_Network_Buffer_Prepare(buf);
    send(connection->socket.socket, buf->bytes, (int) buf->size, 0);
    free(buf);
}

void COMiC_Network_Finalize(
        COMiC_In COMiC_Network_ServerNetInfo server
)
{
    closesocket(server.socket.socket);
    WSACleanup();
}