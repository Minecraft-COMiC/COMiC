#include "network.hpp"

namespace COMiC::Network
{
    void init(ServerNetInfo *server)
    {
        WSADATA wsa;
        printf("Initializing WinSock... ");
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            printf("Failed: %d", WSAGetLastError());
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
            printf("Bind failed with error code: %d", WSAGetLastError());
            exit(1);
        }
        puts("Done");
    }

    void listenToConnections(
            ServerNetInfo server,
            ClientNetInfo *client
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
            printf("accept() failed: %d", WSAGetLastError());
            exit(1);
        }

        puts("Connection accepted");

        int message_length;
        char bytes[512];
        while (true)
        {
            message_length = recv(client->socket.socket, bytes, 512, 0);

            if (message_length > 0)
            {
                Buffer buf((Byte *) bytes, 0, message_length);
                buf.readVarInt();

                receivePacket(client, &buf);
            }
            else if (message_length == 0)
            {
                puts("Connection closed");
                break;
            }
            else
            {
                printf("Failed receiving data from client: %d\n", WSAGetLastError());
                break;
            }
        }
    }

    void sendPacket(
            ClientNetInfo *connection,
            Buffer *buf
    )
    {
        buf->prepare();
        send(connection->socket.socket, (char *) buf->getBytes(), (int) buf->getSize(), 0);
        free(buf);
    }

    void finalize(ServerNetInfo server)
    {
        closesocket(server.socket.socket);
        WSACleanup();
    }

    void sendHTTPGet(const char *server, const char *page, Byte *out, size_t *written)
    {
        *written = 0;

        // Initialize WinInet:
        HINTERNET hInternet = InternetOpenA(
                "",
                INTERNET_OPEN_TYPE_PRECONFIG,
                nullptr,
                nullptr,
                0
        );

        if (hInternet == nullptr)
        {
            fprintf(stderr, "sendHTTPGet(): InternetOpen() failed: %d", GetLastError());
            return;
        }

        // Open HTTP session:
        HINTERNET hConnect = InternetConnectA(
                hInternet,
                server,
                INTERNET_DEFAULT_HTTPS_PORT,
                nullptr,
                nullptr,
                INTERNET_SERVICE_HTTP,
                0,
                0
        );

        if (hConnect == nullptr)
        {
            InternetCloseHandle(hInternet);
            fprintf(stderr, "sendHTTPGet(): InternetConnect() to %s failed: %d", server, GetLastError());
            return;
        }

        // Open request:
        HINTERNET hRequest = HttpOpenRequestA(
                hConnect,
                "GET",
                page,
                nullptr,
                nullptr,
                nullptr,
                INTERNET_FLAG_SECURE,
                0
        );

        if (hRequest == nullptr)
        {
            InternetCloseHandle(hInternet);
            InternetCloseHandle(hConnect);
            fprintf(stderr, "sendHTTPGet(): HttpOpenRequest() to %s%s failed: %d", server, page, GetLastError());
            return;
        }

        // Send request:
        if (HttpSendRequestA(hRequest, nullptr, 0, nullptr, 0) == TRUE)
        {
            Byte buf[1024];
            while (true)
            {
                // Read response:
                DWORD bytesRead;
                BOOL isRead = InternetReadFile(
                        hRequest,
                        buf,
                        sizeof(buf),
                        &bytesRead
                );

                if (isRead == FALSE || bytesRead == 0)
                    break;

                if (out != nullptr)
                    memcpy(out + *written, buf, bytesRead);

                *written += bytesRead;
            }

            if (out != nullptr)
                out[*written] = '\0';
        }
        else
            fprintf(stderr, "sendHTTPGet(): HttpSendRequest() to %s%s failed: %d", server, page, GetLastError());

        // Close request:
        InternetCloseHandle(hRequest);

        // Close session:
        InternetCloseHandle(hConnect);

        // Close WinInet:
        InternetCloseHandle(hInternet);
    }
}