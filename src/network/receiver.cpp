#include <string>
#include <iostream>
#include <COMiC/network.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/util.hpp>

namespace COMiC::Network
{
    // TODO: Make separate handle functions
    void NetManager::receivePacket(ClientNetInfo *connection, Buffer *buf) const
    {
        PacketID packet_id = buf->readPacketID(connection->state);

        std::cout << "/////////////////NEW PACKET/////////////////" << std::endl;
        std::cout << "Id: 0x" << ((packet_id & 0xFF) < 0x10 ? "0" : "") << (packet_id & 0xFF) << std::endl;
        std::cout << "Network state: " << connection->state << std::endl;

        switch (packet_id)
        {
            default:
                break;
            case HANDSHAKE_C2S_PACKET_ID:
                std::cout << "Protocol version: " << buf->readVarInt() << std::endl;

                char serv_address[255];
                buf->readString(serv_address, 255);
                std::cout << "Server address:" << serv_address << std::endl;

                std::cout << "Server port:" << buf->readShort() << std::endl;

                connection->state = (NetworkState) buf->readEnum();
                std::cout << "New state: " << connection->state << std::endl;

                break;
            case HELD_ITEM_CHANGE_S2C_PACKET_ID:
                sendHeldItemChangePacket(connection);
                std::cout << "Held Item Change packet sent" << std::endl;

                break;
            case LOGIN_HELLO_C2S_PACKET_ID:
                connection->username = new char[16];
                buf->readString(connection->username, 16);
                std::cout << "Username: " << connection->username << std::endl;

                sendRequestEncryptionPacket(connection);

                break;
            case LOGIN_KEY_C2S_PACKET_ID:
                I32 enc_len = buf->readVarInt();
                Byte *enc = new Byte[enc_len];
                buf->readByteArray(enc, enc_len);

                I32 token_len = buf->readVarInt();
                Byte *token = new Byte[token_len];
                buf->readByteArray(token, token_len);

                USize len;
                this->rsa->decrypt(enc, enc_len, nullptr, &len);

                Byte key[16];
                this->rsa->decrypt(enc, enc_len, key, &len);

                connection->encrypted = true;
                connection->cipher = new Crypto::AES(key, key);

                // Send HTTP GET to Mojang session servers:
                Crypto::SHA1 sha1;
                Byte digest[SHA_DIGEST_LENGTH];
                sha1.update((Byte *) "", strlen("")); // Server id
                sha1.update(key, sizeof(key)); // AES key
                sha1.update(this->rsa->getEncodedPublicKey(), this->rsa->getEncodedKeySize());
                sha1.final(digest);

                char hexdigest[64];
                Crypto::SHA1::hexdigest(digest, hexdigest);

                std::string response;
                std::string page("/session/minecraft/hasJoined?username=");
                page.append(connection->username);
                page.append("&serverId=");
                page.append(hexdigest);

                sendHTTPGet("sessionserver.mojang.com", page, response);

                // TODO: Proper JSON parser should be used for this
                USize start = 0, end = 0;
                for (USize i = 0, c = 0; i < response.length(); i++)
                {
                    if (response[i] == '\"')
                        c++;

                    if (c == 3 && start == 0)
                        start = i + 1;

                    if (c == 4)
                    {
                        end = i - 1;
                        break;
                    }
                }

                auto uuid_str = response.substr(start, end - start + 1);
                connection->uuid = new Util::UUID(uuid_str, false);

                sendLoginSuccessPacket(connection);
                std::cout << "Login Success packet sent" << std::endl;

                sendGameJoinPacket(connection);
                std::cout << "Join Game packet sent" << std::endl;

                break;
        }

        std::cout << "Bytes = [";
        for (auto i = 0; i < buf->size; i++)
            std::cout << buf->bytes[i] << (i != buf->size - 1 ? ", " : "]");
        std::cout << std::endl;
    }
}