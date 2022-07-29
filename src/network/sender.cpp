#include <nlohmann/json.hpp>
#include <COMiC/network.hpp>
#include <COMiC/crypto.hpp>

namespace COMiC::Network
{
    void ServerNetManager::sendDisconnectPacket(ClientNetInfo &connection, const std::string &reason)
    {
        std::cout << "Disconnect S->C" << std::endl;
        Buffer buf;

        buf.writePacketID(DISCONNECT_S2C_PACKET_ID);
        buf.writeString("{\n    \"text\": \"" + reason + "\"\n}", 32767);

        sendPacket(connection, buf);
    }

    void ServerNetManager::sendRequestEncryptionPacket(ClientNetInfo &connection) const
    {
        std::cout << "Request Encryption S->C" << std::endl;
        Byte nonce[4];
        COMiC::Crypto::secureBytes(nonce, 4);

        Buffer buf;

        buf.writePacketID(LOGIN_HELLO_S2C_PACKET_ID);

        buf.writeString("", 20); // Server id
        // Write public RSA key:
        buf.writeByteArray(this->rsa.getEncodedPublicKey(), (I32) this->rsa.getEncodedKeySize());
        buf.writeByteArray(nonce, 4); // Nonce

        sendPacket(connection, buf);
    }

    void ServerNetManager::sendLoginSuccessPacket(ClientNetInfo &connection)
    {
        std::cout << "Login Success S->C" << std::endl;
        Buffer buf;

        buf.writePacketID(LOGIN_SUCCESS_S2C_PACKET_ID);

        buf.writeString(connection.uuid.toString(), 36);
        buf.writeString(connection.username, 16);

        sendPacket(connection, buf);

        connection.state = PLAY;
    }

    // https://wiki.vg/Server_List_Ping#Status_Response
    void ServerNetManager::sendStatusResponsePacket(ClientNetInfo &connection)
    {
        std::cout << "Query Response S->C" << std::endl;
        std::string str; // String representation of JSON-response
        try
        {
            nlohmann::json response;
            response["version"]["name"] = "1.14.4";
            response["version"]["protocol"] = 498;

            response["players"]["max"] = 20;
            response["players"]["online"] = 0;

            response["description"]["text"] = "A COMiC Server";

            std::string favicon;
            Util::readPNG("favicon.png", favicon);
            Byte *buf = reinterpret_cast<Byte *>(favicon.data() + 16);
            U32 width = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3],
                    height = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + buf[7];

            if (width == 64 && height == 64)
            {
                Crypto::Base64::encode((Byte *) favicon.data(), favicon.length(), favicon);
                response["favicon"] = ("data:image/png;base64," + favicon);
            } else
                std::cerr << "favicon must be exactly 64x64 pixels but instead is "
                          << width << "x" << height << std::endl;

            str = nlohmann::to_string(response);
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }

        Buffer buf;

        buf.writePacketID(QUERY_RESPONSE_S2C_PACKET_ID);
        buf.writeString(str, 32767);

        sendPacket(connection, buf);
    }

    void ServerNetManager::sendPongPacket(ClientNetInfo &connection, I64 payload)
    {
        std::cout << "Pong S->C" << std::endl;
        Buffer buf;

        buf.writePacketID(QUERY_PONG_S2C_PACKET_ID);
        buf.writeLong(payload);

        sendPacket(connection, buf);
    }

    void ServerNetManager::sendGameJoinPacket(ClientNetInfo &connection)
    {
        std::cout << "Game Join S->C" << std::endl;
        Buffer buf;

        buf.writePacketID(GAME_JOIN_S2C_PACKET_ID);

        buf.writeInt(0); // Player Entitiy ID
        buf.write(0); // Gamemode, no hardcore
        buf.writeInt(0); // Overworld raw id
        buf.write(20); // Max players
        buf.writeString("flat", 16); // Generator type
        buf.writeVarInt(10); // View distance
        buf.writeBool(false); // Show debug info

        sendPacket(connection, buf);
    }

    void ServerNetManager::sendSetCompressionPacket(ClientNetInfo &connection, I32 threshold)
    {
        std::cout << "Set Compression S->C" << std::endl;
        Buffer buf;

        buf.writePacketID(LOGIN_COMPRESSION_S2C_PACKET_ID);
        buf.writeVarInt(threshold);

        sendPacket(connection, buf);

        connection.compressed = true;
    }

    void ServerNetManager::sendHeldItemChangePacket(ClientNetInfo &connection)
    {
        Buffer buf;

        buf.writePacketID(HELD_ITEM_CHANGE_S2C_PACKET_ID);
        buf.write(4); // Selected hotbar shot

        sendPacket(connection, buf);
    }
}