#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <COMiC/network.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/util.hpp>

namespace COMiC::Network
{
    // TODO: Make separate handle functions
    void NetworkManager::receivePacket(ClientNetInfo &connection, RecvBuf buf)
    {
        PacketID packet_id = buf.readPacketID(connection.state);
        std::cout << "////////////////|NEW PACKET from ";
        if (connection.username.empty())
            std::cout << "socket " << connection.getSocket();
        else
            std::cout << connection.username;
        std::cout << "|////////////////" << std::endl;

        std::stringstream stream;
        stream << "Id: 0x" << std::setfill('0') << std::setw(2) << std::hex << (packet_id & 0xFF);

        std::cout << stream.str() << std::endl;
        std::cout << "Network state: " << connection.state << std::endl;

        switch (packet_id)
        {
            default:
                break;
            case HANDSHAKE_C2S_PACKET_ID:
                std::cout << "Handshake C->S" << std::endl;
                std::cout << "Protocol version: " << buf.readVarInt() << std::endl;
                std::cout << "Server address: " << buf.readString(255) << std::endl;
                std::cout << "Server port: " << buf.readShort() << std::endl;

                connection.state = (NetworkState) buf.readEnum();
                std::cout << "New state: " << connection.state << std::endl;

                break;
            case LOGIN_HELLO_C2S_PACKET_ID:
                std::cout << "Login Hello C->S" << std::endl;
                connection.username = buf.readString(16);
                sendRequestEncryptionPacket(connection);
                break;
            case LOGIN_KEY_C2S_PACKET_ID:
                std::cout << "Login Key C->S" << std::endl;
                handleEncryptionResponsePacket(connection, buf);

                sendSetCompressionPacket(connection, CONFIG.networkCompressionThreshold());
                sendLoginSuccessPacket(connection);
                disconnect(connection, "Not Implemented"); // Nothing is ready yet for futher client login
                break;
            case QUERY_PING_C2S_PACKET_ID:
                std::cout << "Ping C->S" << std::endl;
                sendPongPacket(connection, buf.readLong());
                break;
            case QUERY_REQUEST_C2S_PACKET_ID:
                std::cout << "Query Request C->S" << std::endl;
                sendStatusResponsePacket(connection);
                break;
            case HELD_ITEM_CHANGE_S2C_PACKET_ID:
                sendHeldItemChangePacket(connection);
                break;
        }

        auto bytes = buf.getBytes();
        std::cout << "Bytes[" << bytes.size() << "] = {";
        for (auto i = 0; i < bytes.size(); i++)
            std::cout << (I32) (I8) bytes[i] << (i != bytes.size() - 1 ? ", " : "}");
        std::cout << std::endl;
    }

    void NetworkManager::handleEncryptionResponsePacket(ClientNetInfo &connection, RecvBuf &buf)
    {
        auto enc = buf.readByteArray();
        auto token = buf.readByteArray();

        auto key = this->rsa.decrypt(enc);

        connection.cipher.init(key.data(), key.data());
        connection.encrypted = true;

        // Send HTTP GET to Mojang session servers:
        Crypto::SHA1 sha1;
        sha1.update(""); // Server id
        sha1.update(key); // AES key
        sha1.update(this->rsa.getEncodedPublicKey()); // Server public key

        std::string page("/session/minecraft/hasJoined?username=");
        page.append(connection.username);
        page.append("&serverId=");
        page.append(Crypto::SHA1::hexdigest(sha1.final().data()));

        if (CONFIG.preventProxyConnections())
        {
            page.append("&ip=");
            page.append(connection.getIP());
        }

        auto response = sendHTTPGet("sessionserver.mojang.com", page);
        try
        {
            auto json = nlohmann::json::parse(response);
            connection.uuid = Util::UUID(json["id"].get<std::string>());
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}