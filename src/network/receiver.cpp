#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <COMiC/network.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/util.hpp>

namespace COMiC::Network
{
    // TODO: Make separate handle functions
    void ServerNetManager::receivePacket(ClientNetInfo &connection, Buffer &buf) const
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

                sendSetCompressionPacket(connection, Config::NETWORK_COMPRESSION_THRESHOLD);
                sendLoginSuccessPacket(connection);
                sendGameJoinPacket(connection);

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

        std::cout << "Bytes[" << buf.size << "] = {";
        for (auto i = 0; i < buf.size; i++)
            std::cout << (I32) (I8) buf.bytes[i] << (i != buf.size - 1 ? ", " : "}");
        std::cout << std::endl;
    }

    void ServerNetManager::handleEncryptionResponsePacket(ClientNetInfo &connection, Buffer &buf) const
    {
        I32 enc_len = buf.readVarInt();
        Byte *enc = new Byte[enc_len];
        buf.readByteArray(enc, enc_len);

        I32 token_len = buf.readVarInt();
        Byte *token = new Byte[token_len];
        buf.readByteArray(token, token_len);

        USize len;
        this->rsa.decrypt(enc, enc_len, nullptr, len);

        Byte key[16];
        this->rsa.decrypt(enc, enc_len, key, len);

        connection.encrypted = true;
        connection.cipher.init(key, key);

        // Send HTTP GET to Mojang session servers:
        Crypto::SHA1 sha1;
        sha1.init();
        Byte digest[SHA_DIGEST_LENGTH];
        sha1.update((Byte *) "", strlen("")); // Server id
        sha1.update(key, sizeof(key)); // AES key
        sha1.update(this->rsa.getEncodedPublicKey(), this->rsa.getEncodedKeySize()); // Server public key
        sha1.final(digest);

        std::string hexdigest;
        Crypto::SHA1::hexdigest(digest, hexdigest);

        std::string response;
        std::string page("/session/minecraft/hasJoined?username=");
        page.append(connection.username);
        page.append("&serverId=");
        page.append(hexdigest);

        if (Config::PREVENT_PROXY_CONNECTIONS)
        {
            page.append("&ip=");
            page.append(connection.getIP());
        }

        sendHTTPGet("sessionserver.mojang.com", page, response);
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