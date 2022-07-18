#include <string>
#include <iostream>
#include <vector>
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
        std::cout << "Creating socket... ";
        if ((server.socket->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            std::cerr << "Could not create socket: " << strerror(errno) << std::endl;
            exit(1);
        }
        std::cout << "Done" << std::endl;

        // Prepare the server structure:
        server.address->address.sin_family = AF_INET;
        inet_pton(AF_INET, DEFAULT_SERVER_IP, &server.address->address.sin_addr.s_addr);
        server.address->address.sin_port = htons(DEFAULT_SERVER_PORT);

        std::cout << "Binding... ";
        if (bind(server.socket->socket, (sockaddr *) &server.address->address, sizeof(server.address->address)) < 0)
        {
            std::cerr << "Bind failed: " << strerror(errno) << std::endl;
            exit(1);
        }
        std::cout << "Done" << std::endl;
    }

    void listenToConnections(const ServerNetManager &server, ClientNetInfo &connection)
    {
        // Listen to incoming connections:
        if (listen(server.socket->socket, 3) < 0)
        {
            std::cerr << "listen() failed: " << strerror(errno) << std::endl;
            exit(1);
        }

        std::cout << "Waiting for incoming connections..." << std::flush;

        // Accept incoming connection:
        socklen_t c = sizeof(connection.address->address);
        connection.socket->socket = (int) accept(server.socket->socket, (sockaddr *) &connection.address->address, &c);
        if (connection.socket->socket < 0)
        {
            std::cerr << "accept() failed: " << strerror(errno) << std::endl;
            exit(1);
        }

        std::cout << "Connection accepted" << std::endl;

        ssize_t message_length;
        Byte bytes[1024];
        std::vector<Byte> msg;
        while (true)
        {
            do
            {
                message_length = recv(connection.socket->socket, bytes, sizeof(bytes), 0);

                if (message_length > 0)
                    msg.insert(msg.end(), bytes, bytes + message_length);
                else break;
            }
            while (message_length == sizeof(bytes));

            if (message_length > 0)
            {
                if (connection.encrypted)
                    connection.cipher.decrypt(msg.data(), msg.size(), msg.data());

                Buffer buf(msg.data(), 0, msg.size());
                msg.clear();
                buf.size = buf.readVarInt();

                if (connection.compressed)
                {
                    I32 data_length = buf.readVarInt();

                    if (data_length > 0)
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
                std::cerr << "Failed receiving data from connection: " << strerror(errno) << std::endl;
                break;
            }
        }
    }

    void sendPacket(ClientNetInfo &connection, Buffer &buf)
    {
        buf.prepare(connection);
        send(connection.socket->socket, buf.bytes + buf.index, (size_t) buf.size, 0);
    }

    void finalize(const ServerNetManager &server)
    {
        close(server.socket->socket);
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

    void sendHTTPGet(const std::string &server, const std::string &page, std::string &out)
    {
        CURL *curl;
        char err[CURL_ERROR_SIZE];

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl == nullptr)
        {
            std::cerr << "sendHTTPGet(): cURL initialization failed" << std::endl;
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, (server + page).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, true);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunc);

        if (curl_easy_perform(curl) != CURLE_OK)
        {
            std::cout << err << std::endl;
            return;
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

}