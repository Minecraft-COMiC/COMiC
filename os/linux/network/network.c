#include "network.h"

void COMiC_Network_Init(
        COMiC_Out COMiC_Network_ServerNetInfo *server
)
{
    printf("Creating socket... ");
    if ((server->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket: %s", strerror(errno));
        exit(1);
    }
    puts("Done");

    // Prepare the server structure:
    server->address.sin_family = AF_INET;
    inet_pton(AF_INET, DEFAULT_SERVER_IP, &server->address.sin_addr.s_addr);
    server->address.sin_port = htons(DEFAULT_SERVER_PORT);

    printf("Binding... ");
    if (bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address)) < 0)
    {
        printf("Bind failed: %s", strerror(errno));
        exit(1);
    }
    puts("Done");
}

void COMiC_Network_ListenToConnections(
        COMiC_In COMiC_Network_ServerNetInfo server,
        COMiC_In COMiC_Network_ClientNetInfo *client,
        COMiC_In void (*onPacketReceive)(COMiC_Network_ClientNetInfo *, COMiC_Network_ByteBuffer *)
)
{
    // Listen to incoming connections:
    if (listen(server.socket, 3) < 0)
    {
        printf("Listen failed: %s", strerror(errno));
        exit(1);
    }

    puts("Waiting for incoming connections...");

    // Accept incoming connection:
    socklen_t c = sizeof(client->address);
    client->socket = (int) accept(server.socket, (struct sockaddr *) &client->address, &c);
    if (client->socket < 0)
    {
        printf("Accept failed with error code : %s", strerror(errno));
        exit(1);
    }

    puts("Connection accepted");

    ssize_t message_length;
    char bytes[512];
    while (420 > 69)
    {
        message_length = recv(client->socket, bytes, 512, 0);

        if (message_length > 0)
        {
            COMiC_Network_ByteBuffer buf = {.bytes = bytes, .index = 0, .size = COMiC_Network_Buffer_ReadVarInt(
                    &buf)};
            onPacketReceive(client, &buf);
        }
        else if (message_length == 0)
        {
            puts("Connection closed");
            break;
        }
        else
        {
            printf("Failed receiving data from client! Error code: %s\n", strerror(errno));
            break;
        }
    }
}

void COMiC_Network_SendPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_ByteBuffer *buf
)
{
    COMiC_Network_Buffer_Prepare(buf);
    send(connection->socket, buf->bytes, (size_t) buf->size, 0);
    free(buf);
}

void COMiC_Network_Finalize(
        COMiC_In COMiC_Network_ServerNetInfo server
)
{
    close(server.socket);
}