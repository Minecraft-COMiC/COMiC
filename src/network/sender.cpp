#include <nlohmann/json.hpp>
#include <COMiC/network.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/io.hpp>

namespace COMiC::Network
{
    void NetworkManager::sendDisconnectPacket(ClientNetInfo &connection, const std::string &reason)
    {
        std::cout << "Disconnect S->C" << std::endl;
        SendBuf buf;

        buf.writePacketID(DISCONNECT_S2C_PACKET_ID);
        buf.writeString("{\n    \"text\": \"" + reason + "\"\n}", 32767);

        sendPacket(connection, buf);
    }

    void NetworkManager::sendRequestEncryptionPacket(ClientNetInfo &connection)
    {
        std::cout << "Request Encryption S->C" << std::endl;
        ByteVector nonce(4);
        COMiC::Crypto::secureBytes(nonce);

        SendBuf buf;

        buf.writePacketID(LOGIN_HELLO_S2C_PACKET_ID);

        buf.writeString("", 20); // Server id
        buf.writeByteArray(this->rsa.getEncodedPublicKey()); // Public RSA key
        buf.writeByteArray(nonce); // Nonce

        sendPacket(connection, buf);
    }

    void NetworkManager::sendLoginSuccessPacket(ClientNetInfo &connection)
    {
        std::cout << "Login Success S->C" << std::endl;
        SendBuf buf;

        buf.writePacketID(LOGIN_SUCCESS_S2C_PACKET_ID);

        buf.writeString(connection.uuid.toString(), 36);
        buf.writeString(connection.username, 16);

        sendPacket(connection, buf);

        connection.state = PLAY;
    }

    // https://wiki.vg/Server_List_Ping#Status_Response
    void NetworkManager::sendStatusResponsePacket(ClientNetInfo &connection)
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

            try
            {
                auto favicon = IO::readFile("favicon.png");
                auto buf = (Byte *) (favicon.data() + 16);
                U32 width = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3],
                    height = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + buf[7];

                if (width == 64 && height == 64)
                    response["favicon"] = ("data:image/png;base64," + Crypto::Base64::encode(favicon));
                else
                    std::cerr << "favicon must be exactly 64x64 pixels but instead is "
                              << width << "x" << height << std::endl;
            }
            catch (IO::IOError &)
            {}

            str = nlohmann::to_string(response);
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }

        SendBuf buf;

        buf.writePacketID(QUERY_RESPONSE_S2C_PACKET_ID);
        buf.writeString(str, 32767);

        sendPacket(connection, buf);
    }

    void NetworkManager::sendPongPacket(ClientNetInfo &connection, I64 payload)
    {
        std::cout << "Pong S->C" << std::endl;
        SendBuf buf;

        buf.writePacketID(QUERY_PONG_S2C_PACKET_ID);
        buf.writeLong(payload);

        sendPacket(connection, buf);
    }

    void NetworkManager::sendSetCompressionPacket(ClientNetInfo &connection, I32 threshold)
    {
        std::cout << "Set Compression S->C" << std::endl;
        SendBuf buf;

        buf.writePacketID(LOGIN_COMPRESSION_S2C_PACKET_ID);
        buf.writeVarInt(threshold);

        sendPacket(connection, buf);

        connection.compressed = true;
    }

    void NetworkManager::sendHeldItemChangePacket(ClientNetInfo &connection)
    {
        SendBuf buf;

        buf.writePacketID(HELD_ITEM_CHANGE_S2C_PACKET_ID);
        buf.write(4); // Selected hotbar shot

        sendPacket(connection, buf);
    }
}