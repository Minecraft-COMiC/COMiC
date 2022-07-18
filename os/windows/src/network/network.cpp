#include <cstdio>
#include <string>
#include <vector>
#include "network.hpp"
#include "COMiC/core/_os.h"

namespace COMiC::Network
{
    ClientNetInfo::ClientNetInfo()
    {
        this->socket = new OS::Socket();
        this->address = new OS::InetAddr();
    }

    ClientNetInfo::~ClientNetInfo()
    {
        delete this->socket;
        delete this->address;
    }

    ServerNetManager::ServerNetManager()
    {
        this->address = new OS::InetAddr();
        this->socket = new OS::Socket();
    }

    ServerNetManager::~ServerNetManager()
    {
        delete this->address;
        delete this->socket;
    }

    void init(ServerNetManager &server)
    {
        WSADATA wsa;

        std::cout << "Initializing WinSock... ";
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            std::cerr << "Could not initialize WinSock: " << WSAGetLastError() << std::endl;
            exit(1);
        }
        std::cout << "Done" << std::endl;

        std::cout << "Creating socket... ";
        if ((server.socket->socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
            std::cerr << "Could not create socket: " << WSAGetLastError() << std::endl;
            exit(1);
        }
        std::cout << "Done" << std::endl;

        // Prepare the server structure:
        server.address->address.sin_family = AF_INET;
        InetPton(AF_INET, DEFAULT_SERVER_IP, &server.address->address.sin_addr.s_addr);
        server.address->address.sin_port = htons(DEFAULT_SERVER_PORT);

        std::cout << "Binding... ";
        if (bind(
                server.socket->socket,
                (sockaddr *) &server.address->address,
                sizeof(server.address->address)
        ) == SOCKET_ERROR)
        {
            std::cerr << "Bind failed with error code: " << WSAGetLastError() << std::endl;
            exit(1);
        }
        std::cout << "Done" << std::endl;
    }

    void listenToConnections(const ServerNetManager &server, ClientNetInfo &connection)
    {
        // Listen to incoming connections:
        if (listen(server.socket->socket, 3) == SOCKET_ERROR)
        {
            std::cerr << "listen() failed with error code: " << WSAGetLastError() << std::endl;
            exit(1);
        }

        std::cout << "Waiting for incoming connections..." << std::flush;

        // Accept incoming connection:
        int c = sizeof(connection.address->address);
        connection.socket->socket = (int) accept(server.socket->socket, (sockaddr *) &connection.address->address, &c);
        if (connection.socket->socket == INVALID_SOCKET)
        {
            std::cerr << "accept() failed: " << WSAGetLastError() << std::endl;
            exit(1);
        }

        std::cout << "Connection accepted" << std::endl;

        // TODO: Receive full data
        int message_length;
        Byte bytes[1024];
        std::vector<Byte> msg;
        while (true)
        {
            do
            {
                message_length = recv(connection.socket->socket, (char *) bytes, sizeof(bytes), 0);

                if (message_length > 0)
                    msg.insert(msg.end(), bytes, bytes + message_length);
                else break;
            }
            while (message_length > sizeof(bytes));

            if (message_length > 0)
            {
                if (connection.encrypted)
                    connection.cipher.decrypt(msg.data(), msg.size(), msg.data());

                Buffer buf(msg.data(), 0, msg.size());
                msg.clear();
                buf.size = buf.readVarInt();

                if (connection.compressed)
                {
                    I32 data_len = buf.readVarInt();

                    if (data_len > 0)
                    {
                        std::string inflated;
                        connection.inflater.decompress(buf.bytes + buf.index, buf.size - buf.index, inflated);
                        memcpy(buf.bytes, inflated.data(), inflated.length());
                        buf.index = 0;
                    }
                }

                server.receivePacket(connection, buf);
            }
            else if (message_length == 0)
            {
                std::cout << "Connection closed" << std::endl;
                break;
            }
            else
            {
                std::cerr << "Failed receiving data from connection: " << WSAGetLastError() << std::endl;
                break;
            }
        }
    }

    void sendPacket(ClientNetInfo &connection, Buffer &buf)
    {
        buf.prepare(connection);
        send(connection.socket->socket, (char *) (buf.bytes + buf.index), (int) buf.size, 0);
    }

    void finalize(const ServerNetManager &server)
    {
        closesocket(server.socket->socket);
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
            std::cerr << "sendHTTPGet(): InternetOpen() failed: " << GetLastError() << std::endl;
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
            std::cerr << "sendHTTPGet(): InternetConnect() to " << server << " failed: " << GetLastError() << std::endl;
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

            std::cerr << "sendHTTPGet(): HttpOpenRequest() to " << server << page
                      << " failed: " << GetLastError() << std::endl;
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
            std::cerr << "sendHTTPGet(): HttpSendRequest() to " << server << page
                      << " failed: " << GetLastError() << std::endl;

        // Close request:
        InternetCloseHandle(hRequest);

        // Close session:
        InternetCloseHandle(hConnect);

        // Close WinInet:
        InternetCloseHandle(hInternet);
    }
}