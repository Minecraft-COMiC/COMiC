#include <cstdio>
#include <string>
#include <array>
#include <utility>
#include <vector>
#include <thread>
#include "network.hpp"
#include "COMiC/core/_os.h"

namespace COMiC::Network
{
    std::string NetworkError::getNativeError()
    {
        DWORD errorMessageID = GetLastError();
        if (errorMessageID == 0)
            return {};

        LPSTR messageBuffer = nullptr;
        auto size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            errorMessageID,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            (LPSTR) &messageBuffer,
            0,
            nullptr
        );

        if (size <= 0)
            return {};

        std::string ret(messageBuffer, size);

        LocalFree(messageBuffer);

        return ret;
    }

    ClientNetInfo::ClientNetInfo(Socket socket, std::string address) : socket(socket), address(std::move(address))
    {
    }

    [[nodiscard]] const std::string &ClientNetInfo::getIP() const
    {
        return this->address;
    }

    [[nodiscard]] Socket ClientNetInfo::getSocket() const
    {
        return this->socket;
    }

    enum
    {
        IOCP_THREADS = 4
    };

    HANDLE hIOCP = INVALID_HANDLE_VALUE;
    std::array<HANDLE, IOCP_THREADS> threads;
    CRITICAL_SECTION criticalSection;
    LPFN_ACCEPTEX acceptExFunc = nullptr;
    LPFN_GETACCEPTEXSOCKADDRS getAcceptedSockAddrsFunc = nullptr;
    DWORD pageSize = 0;
    std::map<Socket, I16> pending; // socket <-> number of pending operations (send(), recv(), accept(), receivePacket())
    bool netDown = false;

    inline void incOpCnt(Socket s)
    {
        EnterCriticalSection(&criticalSection);
        pending[s]++;
        LeaveCriticalSection(&criticalSection);

        WakeByAddressAll(&pending[s]);
    }

    inline void decOpCnt(Socket s)
    {
        EnterCriticalSection(&criticalSection);
        pending[s]--;
        LeaveCriticalSection(&criticalSection);

        WakeByAddressAll(&pending[s]);
    }

    IfError addNewClientSocket(bool lock = false)
    {
        auto s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s == INVALID_SOCKET)
        {
            std::cerr << "Could not create client socket: " << WSAGetLastError() << std::endl;
            return FAIL;
        }

        auto data = new OS::ClientExtraData();
        memset(&data->overlapped, 0, sizeof(OVERLAPPED));
        data->socket = s;
        data->operation = OS::IOCPOperation::ACCEPT;
        data->bytes.reserve(100);
        data->buffer.buf = (CHAR *) data->bytes.data();
        data->buffer.len = (ULONG) data->bytes.capacity();
        data->connection = nullptr;

        if (acceptExFunc(
            NETSERVICE.getSocket(),
            s,
            data->buffer.buf,
            0, // Don't wait for bytes
            sizeof(sockaddr_in) + 16,
            sizeof(sockaddr_in) + 16,
            &data->processed,
            &data->overlapped
        ) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        {
            std::cerr << "AcceptEx() failed: " << WSAGetLastError() << std::endl;
            delete data;
            ::closesocket(s);

            return FAIL;
        }

        // +1 accept() operation
        if (lock)
            EnterCriticalSection(&criticalSection);

        pending[NETSERVICE.getSocket()]++;
        pending.emplace(s, 0);

        if (lock)
        {
            LeaveCriticalSection(&criticalSection);
            WakeByAddressAll(&pending[NETSERVICE.getSocket()]);
        }

        return SUCCESS;
    }

    DWORD WINAPI Work(LPVOID ctx)
    {
        auto iocp = (HANDLE) ctx;
//        std::vector<Byte> recvbuf(pageSize); // Receive buffer
        DWORD bytesTransferred;
        OS::ClientExtraData *data;
        ULONG_PTR key;
        LPWSAOVERLAPPED ovrlppd;
        DWORD flags;
        bool cancelled, failed;
        auto serverSocket = NETSERVICE.getSocket();
        I32 packetSize;

        while (true)
        {
            cancelled = false;
            failed = false;
            flags = 0;

            if (GetQueuedCompletionStatus(
                iocp,
                &bytesTransferred,
                &key,
                &ovrlppd,
                INFINITE
            ) == FALSE)
            {
                if (GetLastError() == ERROR_OPERATION_ABORTED)
                    cancelled = true;
                else
                    failed = true;
            }

            if (netDown)
                break;

            data = CONTAINING_RECORD(ovrlppd, OS::ClientExtraData, overlapped);
            switch (data->operation)
            {
                case OS::IOCPOperation::SEND:
                    // -1 send() operation
                    decOpCnt(data->socket);

                    data->processed += bytesTransferred;
                    data->buffer.buf += bytesTransferred;
                    data->buffer.len -= bytesTransferred;
                    // Send remaining data, if present
                    if (data->processed < data->buffer.len && data->connection->open)
                    {
                        if (WSASend(
                            data->socket,
                            &data->buffer,
                            1,
                            nullptr,
                            0,
                            &data->overlapped,
                            nullptr
                        ) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
                        {
                            NETSERVICE.disconnect(*data->connection);
                            delete data;
                            continue;
                        }

                        // +1 send() operation
                        incOpCnt(data->socket);

                        continue;
                    }

                    // Destroy data as it's no longer needed:
                    delete data;

                    break;
                case OS::IOCPOperation::RECEIVE:
                    // -1 recv() operation
                    decOpCnt(data->socket);

                    if (failed)
                    {
                        NETSERVICE.disconnect(*data->connection);
                        delete data;
                        continue;
                    }

                    if (cancelled || !data->connection->open)
                    {
                        delete data;
                        continue;
                    }

                    // Client closed the connection:
                    if (bytesTransferred == 0)
                    {
                        NETSERVICE.disconnect(*data->connection);
                        delete data;
                        continue;
                    }

                    packetSize = RecvBuf::getPacketSize(
                        *data->connection,
                        (Byte *&) data->buffer.buf,
                        bytesTransferred
                    );
                    if (packetSize <= 0)
                    {
                        NETSERVICE.disconnect(*data->connection, "Invalid packet size");
                        delete data;
                        continue;
                    }

                    // TODO: Handle big packets
                    try
                    {
                        // Don't erase the connection until all packets have been processed
                        incOpCnt(data->socket);
                        NETSERVICE.receivePacket(
                            *data->connection,
                            RecvBuf((Byte *) data->buffer.buf, packetSize, *data->connection)
                        );
                        decOpCnt(data->socket);
                    }
                    catch (std::exception &e)
                    {
                        std::cerr << "Exception encountered whilst processing packet: " << e.what() << std::endl;
                        NETSERVICE.disconnect(*data->connection, "Bad packet");
                        delete data;
                        continue;
                    }

                    // Issue another RECEIVE:
                    if (WSARecv(
                        data->socket,
                        &data->buffer,
                        1,
                        nullptr,
                        &flags,
                        &data->overlapped,
                        nullptr
                    ) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
                    {
                        NETSERVICE.disconnect(*data->connection);
                        delete data;
                        continue;
                    }

                    // +1 recv() operation
                    incOpCnt(data->socket);

                    break;
                case OS::IOCPOperation::ACCEPT:
                    // -1 accept() operation:
                    decOpCnt(serverSocket);

                    if (!cancelled)
                    {
                        EnterCriticalSection(&criticalSection);
                        (void) addNewClientSocket();
                        LeaveCriticalSection(&criticalSection);
                        WakeByAddressAll(&pending[serverSocket]);
                    }
                    else
                    {
                        delete data;
                        continue;
                    }

                    if (failed)
                    {
                        NETSERVICE.closeSocket(data->socket, false);
                        delete data;
                        continue;
                    }

                    if (::setsockopt(
                        data->socket,
                        SOL_SOCKET,
                        SO_UPDATE_ACCEPT_CONTEXT,
                        (char *) &serverSocket,
                        sizeof(SOCKET)
                    ) == SOCKET_ERROR)
                    {
                        std::cerr << "Client socket could not inherit properties of server one: "
                                  << WSAGetLastError() << std::endl;
                        NETSERVICE.closeSocket(data->socket, false);
                        delete data;
                        continue;
                    }

                    // Get client IP address:
                    INT tmp;
                    sockaddr *local, *remote;
                    getAcceptedSockAddrsFunc(
                        &data->buffer.buf,
                        0,
                        sizeof(SOCKADDR_IN) + 16,
                        sizeof(SOCKADDR_IN) + 16,
                        &local,
                        &tmp,
                        &remote,
                        &tmp
                    );

                    char ip[INET_ADDRSTRLEN];
                    InetNtop(AF_INET, remote, ip, INET_ADDRSTRLEN);

                    // Add socket to our client list:
                    EnterCriticalSection(&criticalSection);
                    data->connection = &NETSERVICE.addClient(data->socket, ip);
                    LeaveCriticalSection(&criticalSection);

                    // Add socket to I/OCP:
                    hIOCP = CreateIoCompletionPort((HANDLE) data->socket, hIOCP, 0, IOCP_THREADS);

                    // Issue initial RECEIVE request on the new client socket:
                    data->operation = OS::IOCPOperation::RECEIVE;
                    data->bytes.reserve(pageSize);
                    data->buffer.buf = (CHAR *) data->bytes.data();
                    data->buffer.len = pageSize;
                    if (WSARecv(
                        data->socket,
                        &data->buffer,
                        1,
                        nullptr,
                        &flags,
                        &data->overlapped,
                        nullptr
                    ) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
                    {
                        std::cerr << "recv() failed: " << WSAGetLastError() << std::endl;
                        NETSERVICE.disconnect(*data->connection);
                        delete data;
                        continue;
                    }

                    // +1 recv() operation
                    incOpCnt(data->socket);

                    break;
                default:
                    std::cerr << "Unknown IOCP operation id: " << data->operation << std::endl;
                    delete data;
                    break;
            }
        }

        return 0;
    }

    NetworkManager::NetworkManager()
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            throw NetworkError("Could not initialize WinSock");

        std::cout << "Using `WinSock " <<
                  (I32) HIBYTE(wsa.wVersion) << '.' << (I32) LOBYTE(wsa.wVersion) <<
                  "` as COMiC::Network API" << std::endl;

        std::cout << "Prepairing server socket... ";
        auto s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s == INVALID_SOCKET)
            throw NetworkError("Could not create socket");

        this->socket = s;

        // Disable send buffer:
        int off = 0;
        if (::setsockopt(this->socket, SOL_SOCKET, SO_SNDBUF, (char *) &off, sizeof(off)) == SOCKET_ERROR)
            throw NetworkError("Failed to disable send buffer");

//        BOOL enabled = TRUE;
//        if (::setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR, (char *) &enabled, sizeof(enabled)) == SOCKET_ERROR)
//            throw NetworkError("Unable to allow reuse of local addresses");

        // Set to non-blocking:
        u_long on = 1;
        if (::ioctlsocket(this->socket, FIONBIO, &on) == SOCKET_ERROR)
            throw NetworkError("Failed to set socket to non-blocking mode");
        std::cout << "Done" << std::endl;

        // Prepare the server structure:
        std::cout << "Binding... ";
        sockaddr_in address{};
        address.sin_family = AF_INET;
        InetPton(AF_INET, CONFIG.serverIP().c_str(), &address.sin_addr.s_addr);
        address.sin_port = htons(CONFIG.serverPort());

        if (bind(
            this->socket,
            (sockaddr *) &address,
            sizeof(address)
        ) == SOCKET_ERROR)
            throw NetworkError("bind() failed");
        std::cout << "Done" << std::endl;

        InitializeCriticalSection(&criticalSection);

        hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, IOCP_THREADS);
        if (hIOCP == nullptr)
            throw NetworkError("Failed to create I/O Completion Port");

        DWORD t_id;
        for (auto &thread: threads)
        {
            thread = CreateThread(nullptr, 0, Work, hIOCP, CREATE_SUSPENDED, &t_id);
            if (thread == nullptr)
                throw NetworkError("Could not create IOCP worker thread");

            if (FAILED(SetThreadDescription(thread, L"COMiC IOCP Worker")))
                throw NetworkError("Could not set thread description");
        }

        // Load AcceptEx() & GetAcceptExSockAddrs():
        GUID guid = WSAID_ACCEPTEX;
        DWORD dwBytes;
        if (WSAIoctl(
            this->socket,
            SIO_GET_EXTENSION_FUNCTION_POINTER,
            &guid,
            sizeof(guid),
            &acceptExFunc,
            sizeof(acceptExFunc),
            &dwBytes,
            nullptr,
            nullptr
        ) == SOCKET_ERROR)
            throw NetworkError("Failed to get AcceptEx() address");

        guid = WSAID_GETACCEPTEXSOCKADDRS;
        if (WSAIoctl(
            this->socket,
            SIO_GET_EXTENSION_FUNCTION_POINTER,
            &guid,
            sizeof(guid),
            &getAcceptedSockAddrsFunc,
            sizeof(getAcceptedSockAddrsFunc),
            &dwBytes,
            nullptr,
            nullptr
        ) == SOCKET_ERROR)
            throw NetworkError("Failed to get GetAcceptExSockAddrs() address");

        // Get page size to use fewer recv() calls:
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        pageSize = info.dwPageSize;
    }

    void NetworkManager::start()
    {
        // Work to incoming connections:
        if (::listen(this->socket, SOMAXCONN) == SOCKET_ERROR)
            throw NetworkError("listen() failed");

        // Add listener socket to IOCP for AcceptEx():
        hIOCP = CreateIoCompletionPort((HANDLE) this->socket, hIOCP, 0, IOCP_THREADS);

        // Make sockets to accept:
        pending.emplace(this->socket, 0);
        for (auto i = 0U; i < CONFIG.maxPlayers(); i++)
            if (addNewClientSocket())
                break;

        // Start IOCP workers after all sockets were created:
        for (auto &thread: threads)
            ResumeThread(thread);

        std::cout << "Server successfully started on " << CONFIG.serverIP() << ':' << CONFIG.serverPort() << std::endl;
    }

    void NetworkManager::sendPacket(ClientNetInfo &connection, SendBuf &buf)
    {
        if (!connection.open)
            return;

        buf.prepare(connection);

        auto *data = new OS::ClientExtraData();
        memset(&data->overlapped, 0, sizeof(OVERLAPPED));
        data->socket = connection.getSocket();
        data->operation = OS::IOCPOperation::SEND;
        std::swap(data->bytes, buf.getBytes());
        data->buffer.buf = (CHAR *) (data->bytes.data() + buf.getIndex());
        data->buffer.len = (ULONG) buf.getSize();
        data->processed = 0;

        if (WSASend(
            data->socket,
            &data->buffer,
            1,
            nullptr,
            0,
            &data->overlapped,
            nullptr
        ) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        {
            std::cerr << "send() failed: " << WSAGetLastError() << std::endl;
            this->disconnect(connection);
            delete data;
            return;
        }

        // +1 send() operation
        EnterCriticalSection(&criticalSection);
        pending[data->socket]++;
        LeaveCriticalSection(&criticalSection);
        WakeByAddressAll(&pending[data->socket]);
    }

    void NetworkManager::closeSocket(Socket s, bool connected)
    {
        // Socket was closed already
        if (pending.find(s) == pending.end())
            return;

        // Wait until all operations on the socket complete / get cancelled, then close it
        if (connected)
        {
            CancelIoEx((HANDLE) s, nullptr);

            I16 zero = 0;
            while (pending[s] > 0)
                if (WaitOnAddress(&pending[s], &zero, sizeof(pending[s]), 1000) == FALSE)
                {
                    std::cerr << "Forcibly closed socket " << s << " with " << pending[s] << " operations remaining ("
                              << GetLastError() << ')' << std::endl;
                    break;
                }
        }

        ::shutdown(s, SD_BOTH);
        ::closesocket(s);

        EnterCriticalSection(&criticalSection);
        pending.erase(s);
        if (connected)
            this->clients.erase(s);
        LeaveCriticalSection(&criticalSection);
    }

    void NetworkManager::disconnect(ClientNetInfo &connection, const std::string &reason)
    {
        if (!connection.open)
            return;

        if (!reason.empty())
            sendDisconnectPacket(connection, reason);

        connection.open = false;
        closeSocket(connection.getSocket(), true);

        std::cout << "Connection closed" << std::endl;
    }

    void NetworkManager::end()
    {
        this->closeSocket(this->socket, true);

        while (!this->clients.empty())
            this->disconnect(const_cast<ClientNetInfo &>(this->clients.begin()->second), "Server closed");

        while (!pending.empty())
            this->closeSocket(pending.begin()->first, false);

        netDown = true;

        // Cause IOCP Worker threads to exit:
        for (auto &thread: threads)
            PostQueuedCompletionStatus(hIOCP, 0, 0, nullptr);

        if (WaitForMultipleObjects(IOCP_THREADS, threads.data(), TRUE, 1000) != WAIT_OBJECT_0)
            std::cerr << "COMiC: IOCP Worker threads did not exit properly: " << GetLastError() << std::endl;
    }

    NetworkManager::~NetworkManager()
    {
        std::for_each(threads.begin(), threads.end(), [](HANDLE &thread) { CloseHandle(thread); });

        CloseHandle(hIOCP);
        DeleteCriticalSection(&criticalSection);
        WSACleanup();
    }

    std::string NetworkManager::sendHTTPGet(const std::string &server, const std::string &page)
    {
        // Initialize WinInet:
        std::shared_ptr<void> hInternet(
            InternetOpenA(
                "",
                INTERNET_OPEN_TYPE_PRECONFIG,
                nullptr,
                nullptr,
                0
            ),
            InternetCloseHandle
        );

        if (hInternet == nullptr)
            throw NetworkError("InternetOpen() failed");

        // Open HTTP session:
        std::shared_ptr<void> hConnect(
            InternetConnectA(
                hInternet.get(),
                server.c_str(),
                INTERNET_DEFAULT_HTTPS_PORT,
                nullptr,
                nullptr,
                INTERNET_SERVICE_HTTP,
                0,
                0
            ),
            InternetCloseHandle
        );

        if (hConnect == nullptr)
            throw NetworkError("Could not connect to `" + server + page + '`');

        // Open request:
        std::shared_ptr<void> hRequest(
            HttpOpenRequestA(
                hConnect.get(),
                "GET",
                page.c_str(),
                nullptr,
                nullptr,
                nullptr,
                INTERNET_FLAG_SECURE,
                0
            ),
            InternetCloseHandle
        );

        if (hRequest == nullptr)
            throw NetworkError("Unable to open HTTP GET request to `" + server + page + '`');

        // Send request:
        std::string out;
        if (HttpSendRequestA(hRequest.get(), nullptr, 0, nullptr, 0) == TRUE)
        {
            std::shared_ptr<char> buf(new char[pageSize]);
            while (true)
            {
                // Read response:
                DWORD bytesRead;
                BOOL read = InternetReadFile(
                    hRequest.get(),
                    buf.get(),
                    pageSize,
                    &bytesRead
                );

                if (read == FALSE || bytesRead == 0)
                    break;

                out.append(buf.get(), bytesRead);
            }
        }
        else
            throw NetworkError("HTTP GET request to `" + server + page + "` failed");

        return out;
    }
}