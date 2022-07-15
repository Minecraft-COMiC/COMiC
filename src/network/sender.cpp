#include <COMiC/network.hpp>
#include <COMiC/crypto.hpp>

namespace COMiC::Network
{
    void NetManager::sendRequestEncryptionPacket(ClientNetInfo *connection) const
    {
        Byte nonce[4];
        COMiC::Crypto::secureBytes(nonce, 4);

        Buffer buf;

        buf.writePacketID(LOGIN_HELLO_S2C_PACKET_ID);

        buf.writeString("", 20); // Server id
        // Write public RSA key:
        buf.writeByteArray(this->rsa->getEncodedPublicKey(), (I32) this->rsa->getEncodedKeySize());
        buf.writeByteArray(nonce, 4); // Nonce

        sendPacket(connection, &buf);
    }

    void NetManager::sendLoginSuccessPacket(ClientNetInfo *connection)
    {
        Buffer buf;

        buf.writePacketID(LOGIN_SUCCESS_S2C_PACKET_ID);

        buf.writeString(connection->uuid->toString().c_str(), 36);
        buf.writeString(connection->username, 16);

        sendPacket(connection, &buf);

        connection->state = PLAY;
    }

    void NetManager::sendGameJoinPacket(ClientNetInfo *connection)
    {
        Buffer buf;

        buf.writePacketID(GAME_JOIN_S2C_PACKET_ID);

        buf.writeInt(0); // Player Entitiy ID
        buf.write(0); // Gamemode, no hardcore
        buf.writeInt(0); // Overworld raw id
        buf.write(20); // Max players
        buf.writeString("flat", 16); // Generator type
        buf.writeVarInt(10); // View distance
        buf.writeBool(false); // Show debug info

        sendPacket(connection, &buf);
    }

    void NetManager::sendHeldItemChangePacket(ClientNetInfo *connection)
    {
        Buffer buf;

        buf.writePacketID(HELD_ITEM_CHANGE_S2C_PACKET_ID);

        buf.write(4); // Selected hotbar shot

        sendPacket(connection, &buf);
    }
}