#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <poll.h>
#include <curl/curl.h>
#include "network.hpp"
#include "COMiC/core/_os.h"

namespace COMiC::Network
{
    ClientNetInfo::ClientNetInfo()
    {
        this->socket = new OS::Socket();
        this->address = new OS::InetAddr();
    }

    ClientNetInfo::ClientNetInfo(const ClientNetInfo &other) : ClientNetInfo()
    {
        this->socket->socket = other.socket->socket;
        this->address->address = other.address->address;
        this->state = other.state;
        this->username = other.username;
        this->uuid = other.uuid;
        this->encrypted = other.encrypted;
        this->compressed = other.compressed;
    }

    [[nodiscard]] char *ClientNetInfo::getIP() const
    {
        return inet_ntoa(this->address->address.sin_addr);
    }

    [[nodiscard]] U16 ClientNetInfo::getSocket() const
    {
        return this->socket->socket;
    }

    ClientNetInfo::~ClientNetInfo()
    {
        delete this->socket;
        delete this->address;
    }

    ServerNetManager::~ServerNetManager()
    {
        delete this->address;
        delete this->socket;
    }

    IfError init()
    {
        std::cout << "Preparing server socket... ";
        INSTANCE.socket = new OS::Socket();
        if ((INSTANCE.socket->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
            std::cerr << "Could not create socket: " << strerror(errno) << std::endl;
            return FAIL;
        }

        // Prepare server structure:
        int on = 1;
        if (setsockopt(INSTANCE.socket->socket, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) < 0)
        {
            std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
            return FAIL;
        }

        if (fcntl(INSTANCE.socket->socket, F_SETFL, fcntl(INSTANCE.socket->socket, F_GETFL) | O_NONBLOCK) < 0)
        {
            std::cerr << "Failed to set server socket to non-blocking: " << strerror(errno) << std::endl;
            return FAIL;
        }
        std::cout << "Done" << std::endl;

        std::cout << "Binding... ";
        INSTANCE.address = new OS::InetAddr();
        INSTANCE.address->address.sin_family = AF_INET;
        inet_pton(AF_INET, Config::SERVER_IP.c_str(), &INSTANCE.address->address.sin_addr.s_addr);
        INSTANCE.address->address.sin_port = htons(Config::SERVER_PORT);

        if (bind(
                INSTANCE.socket->socket,
                (sockaddr *) &INSTANCE.address->address,
                sizeof(INSTANCE.address->address)
        ) < 0)
        {
            std::cerr << "bind() failed: " << strerror(errno) << std::endl;
            return FAIL;
        }
        std::cout << "Done" << std::endl;

        if (INSTANCE.rsa.init(false))
        {
            std::cerr << "Failed to initialize RSA cipher" << std::endl;
            return FAIL;
        }

        INSTANCE.clients.reserve(Config::MAX_PLAYERS);

        std::cout << "Server successfully started on " << Config::SERVER_IP << ":" << Config::SERVER_PORT << std::endl;

        return SUCCESS;
    }

    IfError listenToConnections()
    {
        // Listen to incoming connections:
        if (listen(INSTANCE.socket->socket, SOMAXCONN) < 0)
        {
            std::cerr << "listen() failed: " << strerror(errno) << std::endl;
            return FAIL;
        }

        std::cout << "Waiting for incoming connections..." << std::endl;

        auto fds = new struct pollfd[1 + Config::MAX_PLAYERS];
        for (I32 i = 0; i <= Config::MAX_PLAYERS; i++)
            fds[i].events = POLLIN;

        // Set up server listening socket:
        fds[0].fd = INSTANCE.socket->socket;

        Byte bytes[4096];
        std::vector<Byte> msg;
        while (COMiC::alive)
        {
            // Update client info:
            for (I32 i = 0; i < INSTANCE.clients.size(); i++)
                fds[i + 1].fd = INSTANCE.clients[i].socket->socket;

            // Wait for incoming connections:
            if (poll(fds, INSTANCE.clients.size() + 1, -1) < 0)
            {
                std::cerr << "poll() failed " << strerror(errno) << std::endl;
                break;
            }

            for (I32 i = 0; i <= INSTANCE.clients.size(); i++)
            {
                if (fds[i].revents == 0)
                    continue;

                // Accept new connections:
                if (fds[i].fd == INSTANCE.socket->socket)
                {
                    std::cout << "------------------------" << std::endl;
                    std::cout << "New incoming connection:" << std::endl;
                    int new_socket;
                    do
                    {
                        ClientNetInfo new_connection;
                        socklen_t addrlen = sizeof(new_connection.address->address);
                        new_socket = accept(
                                INSTANCE.socket->socket,
                                (struct sockaddr *) &new_connection.address->address,
                                &addrlen
                        );

                        if (new_socket < 0)
                        {
                            if (errno != EWOULDBLOCK && errno != EAGAIN)
                            {
                                std::cerr << "accept() failed: " << strerror(errno) << std::endl;
                                continue;
                            }
                        }
                        else
                        {
                            if (fcntl(new_socket, F_SETFL, fcntl(new_socket, F_GETFL) | O_NONBLOCK) < 0)
                            {
                                std::cerr << "Unable to set new client socket in non-blocking mode: "
                                          << strerror(errno) << std::endl;
                                close(new_socket);
                                continue;
                            }

                            // Add new fd to the vector:
                            new_connection.socket->socket = new_socket;

                            // Add new socket to the array of sockets:
                            INSTANCE.clients.push_back(new_connection);

                            std::cout << "Socket: " << new_socket << std::endl;
                            std::cout << "IP: " << inet_ntoa(new_connection.address->address.sin_addr) << std::endl;
                            std::cout << "Port: " << ntohs(new_connection.address->address.sin_port) << std::endl;
                            std::cout << "------------------------" << std::endl;
                        }
                    }
                    while (new_socket != -1);
                }
                else // Handle data from client:
                {
                    auto sd = fds[i].fd;
                    auto connection = std::find_if(
                            INSTANCE.clients.begin(),
                            INSTANCE.clients.end(),
                            [&](const ClientNetInfo &c)
                            { return c.socket->socket == sd; }
                    );

                    // Should never happen, I guess...
                    if (connection == INSTANCE.clients.end())
                    {
                        std::cerr << "Unable to find client connection associated with socket " << sd << std::endl;
                        close(sd);
                        continue;
                    }

                    ssize_t message_length;
                    while (true)
                    {
                        message_length = recv(sd, bytes, sizeof(bytes), 0);

                        if (message_length > 0)
                            msg.insert(msg.end(), bytes, bytes + message_length);
                        else break;
                    }

                    // Means that client closed the connection
                    if (message_length == 0)
                    {
                        std::cout << "Connection closed" << std::endl;
                        close(sd);
                        INSTANCE.clients.erase(connection);
                        continue;
                    }

                    if (errno == EAGAIN || errno == EWOULDBLOCK) // Received all data
                    {
                        Buffer buf(msg.data(), 0, msg.size());
                        INSTANCE.receivePacket(*connection, buf);
                    }
                    else // Handle error
                    {
                        std::cerr << "Failed receiving data from connection: " << strerror(errno) << std::endl;
                        close(sd);
                        INSTANCE.clients.erase(connection);
                        continue;
                    }

                    msg.clear();
                }
            }
        }

        delete[] fds;

        return SUCCESS;
    }

    IfError sendPacket(ClientNetInfo &connection, Buffer &buf)
    {
        buf.prepare(connection);
        if (send(connection.socket->socket, buf.bytes + buf.index, (size_t) buf.size, 0) < 0)
        {
            std::cerr << "send() failed: " << strerror(errno) << std::endl;
            return FAIL;
        }

        return SUCCESS;
    }

    void disconnect(ClientNetInfo &connection, const std::string &reason)
    {
        ServerNetManager::sendDisconnectPacket(connection, reason);
        if (close(connection.socket->socket))
            std::cerr << "Failed to close client socket: " << strerror(errno) << std::endl;
        INSTANCE.clients.erase(std::find(INSTANCE.clients.begin(), INSTANCE.clients.end(), connection));
    }

    size_t curlWriteFunc(char *data, size_t size, size_t nmemb, std::string *buffer)
    {
        size_t res = 0;

        if (buffer != nullptr)
        {
            buffer->append(data, size * nmemb);
            res = size * nmemb;
        }

        return res;
    }

    IfError sendHTTPGet(const std::string &server, const std::string &page, std::string &out)
    {
        CURL *curl;
        char err[CURL_ERROR_SIZE];

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl == nullptr)
        {
            std::cerr << "sendHTTPGet(): cURL initialization failed" << std::endl;
            return FAIL;
        }

        curl_easy_setopt(curl, CURLOPT_URL, (server + page).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, true);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunc);

        auto res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        if (res != CURLE_OK)
        {
            std::cout << err << std::endl;
            return FAIL;
        }

        return SUCCESS;
    }

    IfError finalize()
    {
        if (close(INSTANCE.socket->socket) != 0)
        {
            std::cerr << "Failed to close server socket: " << strerror(errno) << std::endl;
            return FAIL;
        }

        return SUCCESS;
    }

}