#include <cstdio>
#include <string>
#include "network.hpp"
#include "COMiC/core/_os.h"

namespace COMiC::Network
{
    void init(NetManager *server)
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
        server->socket = new OS::Socket();

        if ((server->socket->socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
            printf("Could not create socket: %d", WSAGetLastError());
            exit(1);
        }
        puts("Done");

        // Prepare the server structure:
        server->address = new OS::InetAddr();
        server->address->address.sin_family = AF_INET;
        InetPton(AF_INET, DEFAULT_SERVER_IP, &server->address->address.sin_addr.s_addr);
        server->address->address.sin_port = htons(DEFAULT_SERVER_PORT);

        printf("Binding... ");
        if (bind(server->socket->socket, (sockaddr *) &server->address->address, sizeof(server->address->address)) ==
            SOCKET_ERROR)
        {
//            printf("Bind failed with error code: %d", WSAGetLastError());
            wchar_t *s = nullptr;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           nullptr, WSAGetLastError(),
                           MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                           (LPWSTR) &s, 100, nullptr);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);

            exit(1);
        }
        puts("Done");
    }

    void listenToConnections(NetManager server, ClientNetInfo *client)
    {
        // Listen to incoming connections:
        listen(server.socket->socket, 3);

        puts("Waiting for incoming connections...");

        // Accept incoming connection:
        client->address = new OS::InetAddr();
        int c = sizeof(client->address->address);
        client->socket = new OS::Socket();
        client->socket->socket = (int) accept(server.socket->socket, (sockaddr *) &client->address->address, &c);
        if (client->socket->socket == INVALID_SOCKET)
        {
            printf("accept() failed: %d", WSAGetLastError());
            exit(1);
        }

        puts("Connection accepted");

        int message_length;
        Byte bytes[512];
        while (true)
        {
            message_length = recv(client->socket->socket, (char *) bytes, sizeof(bytes), 0);

            if (message_length > 0)
            {
                if (client->encrypted)
                    client->cipher->decrypt(bytes, message_length, bytes);

                Buffer buf(bytes, 0, message_length);
                buf.size = buf.readVarInt();

                server.receivePacket(client, &buf);
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

    void sendPacket(ClientNetInfo *connection, Buffer *buf)
    {
        buf->prepare();

        if (connection->encrypted)
            connection->cipher->encrypt(buf->bytes + buf->index, buf->size, buf->bytes + buf->index);

        send(connection->socket->socket, (char *) (buf->bytes + buf->index), (int) buf->size, 0);
    }

    void finalize(NetManager server)
    {
        closesocket(server.socket->socket);
        delete server.address;
        delete server.socket;
        WSACleanup();
    }

    void sendHTTPGet(const std::string &server, const std::string &page, std::string &out)
    {
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
            fprintf(stderr, "sendHTTPGet(): InternetOpen() failed: %lu", GetLastError());
            return;
        }

        // Open HTTP session:
        HINTERNET hConnect = InternetConnectA(
                hInternet,
                server.c_str(),
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
            fprintf(stderr, "sendHTTPGet(): InternetConnect() to %s failed: %lu", server.c_str(), GetLastError());
            return;
        }

        // Open request:
        HINTERNET hRequest = HttpOpenRequestA(
                hConnect,
                "GET",
                page.c_str(),
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
            fprintf(stderr, "sendHTTPGet(): HttpOpenRequest() to %s%s failed: %lu", server.c_str(), page.c_str(),
                    GetLastError());
            return;
        }

        // Send request:
        if (HttpSendRequestA(hRequest, nullptr, 0, nullptr, 0) == TRUE)
        {
            char buf[1024];
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

                out.append(buf, bytesRead);
            }
        }
        else
            fprintf(stderr, "sendHTTPGet(): HttpSendRequest() to %s%s failed: %lu", server.c_str(), page.c_str(),
                    GetLastError());

        // Close request:
        InternetCloseHandle(hRequest);

        // Close session:
        InternetCloseHandle(hConnect);

        // Close WinInet:
        InternetCloseHandle(hInternet);
    }
}