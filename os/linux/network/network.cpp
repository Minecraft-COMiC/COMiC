#include <curl/curl.h>
#include <string>
#include "network.hpp"

namespace COMiC::Network
{
    void init(ServerNetInfo *server)
    {
        printf("Creating socket... ");
        if ((server->socket.socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("Could not create socket: %s", strerror(errno));
            exit(1);
        }
        puts("Done");

        // Prepare the server structure:
        server->address.address.sin_family = AF_INET;
        inet_pton(AF_INET, DEFAULT_SERVER_IP, &server->address.address.sin_addr.s_addr);
        server->address.address.sin_port = htons(DEFAULT_SERVER_PORT);

        printf("Binding... ");
        if (bind(server->socket.socket, (struct sockaddr *) &server->address, sizeof(server->address)) < 0)
        {
            printf("Bind failed: %s", strerror(errno));
            exit(1);
        }
        puts("Done");
    }

    void listenToConnections(ServerNetInfo server, ClientNetInfo *client)
    {
        // Listen to incoming connections:
        if (
                listen(server
                               .socket.socket, 3) < 0)
        {
            printf("Listen failed: %s",

                   strerror(errno)

            );
            exit(1);
        }

        puts("Waiting for incoming connections...");

// Accept incoming connection:
        socklen_t c = sizeof(client->address);
        client->socket.
                socket = (int) accept(server.socket.socket, (struct sockaddr *) &client->address, &c);
        if (client->socket.socket < 0)
        {
            printf("Accept failed with error code : %s",

                   strerror(errno)

            );
            exit(1);
        }

        puts("Connection accepted");

        ssize_t message_length;
        char bytes[512];
        while (true)
        {
            message_length = recv(client->socket.socket, bytes, 512, 0);

            if (message_length > 0)
            {
                Buffer buf((Byte *) bytes, 0, message_length);
                buf.

                        readVarInt();

                receivePacket(client, &buf
                );
            }
            else if (message_length == 0)
            {
                puts("Connection closed");
                break;
            }
            else
            {
                printf("Failed receiving data from client! Error code: %s\n",

                       strerror(errno)

                );
                break;
            }
        }
    }

    void sendPacket(ClientNetInfo *connection, Buffer *buf)
    {
        buf->

                prepare();

        send(connection
                     ->socket.socket, buf->

                     getBytes(), (size_t)

                     buf->

                             getSize(),

             0);
        free(buf);
    }

    void finalize(ServerNetInfo server    )
    {
        close(server.socket.socket);
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

    void sendHTTPGet(const char *server, const char *page, Byte *out, size_t *written)
    {
        CURL *curl;
        char err[CURL_ERROR_SIZE];

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl == nullptr)
        {
            fprintf(stderr, "sendHTTPGet(): cURL initialization failed.");
            return;
        }

        std::string str;

        char *hostname = new char[strlen(server) + strlen(page) + 1];
        strcpy(hostname, server);
        strcat(hostname, page);

        curl_easy_setopt(curl, CURLOPT_URL, hostname);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, true);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunc);

        if (curl_easy_perform(curl) != CURLE_OK)
        {
            fprintf(stderr, "%s\n", err);
            return;
        }

        *written = str.length() + 1;
        memcpy(out, str.c_str(), *written);

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        delete[] hostname;
    }

}