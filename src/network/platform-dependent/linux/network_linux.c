#include "network_linux.h"

void COMiC_Network_init(COMiC_Network_ServerNetInfo *info)
{
    printf("---Starting COMiC server v%s on Linux---\n", COMiC_VERSION);

    printf("Creating socket... ");
    if ((info->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket: %s", strerror(errno));
        exit(1);
    }
    puts("Done");

    // Prepare the server structure:
    info->address.sin_family = AF_INET;
    inet_pton(AF_INET, DEFAULT_SERVER_IP, &info->address.sin_addr.s_addr);
    info->address.sin_port = htons(DEFAULT_SERVER_PORT);

    printf("Binding... ");
    if (bind(info->socket, (struct sockaddr *) &info->address, sizeof(info->address)) < 0)
    {
        printf("Bind failed: %s", strerror(errno));
        exit(1);
    }
    puts("Done");
}

void COMiC_Network_listen_to_connections(
        COMiC_Network_ServerNetInfo server,
        COMiC_Network_ClientNetInfo *client,
        void (*onPacketReceive)(COMiC_Network_ClientNetInfo *, COMiC_Network_ByteBuffer *)
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
    if (client->socket < 0 )
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
            COMiC_Network_ByteBuffer buf = {.bytes = bytes, .index = 0, .size = COMiC_Network_Buffer_read_var_int(&buf)};
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

void COMiC_Network_send_packet(COMiC_Network_ClientNetInfo *connection, COMiC_Network_ByteBuffer *buf)
{
    COMiC_Network_Buffer_prepare(buf);
    send(connection->socket, buf->bytes, (size_t) buf->size, 0);
    free(buf);
}

void COMiC_Network_cleanup(COMiC_Network_ServerNetInfo server)
{
    close(server.socket);
}