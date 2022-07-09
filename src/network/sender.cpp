#include <COMiC/network.hpp>

namespace COMiC::Network
{
/*
void COMiC_Network_SendRequestEncryptionPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
)
{
    char nonce[4];
    RAND_bytes((unsigned char *) nonce, 4);

    COMiC_Network_Buffer buf = COMiC_Network_Buffer_InitWithDefaultCapacity();

    COMiC_Network_Buffer_WritePacketID(&buf, LOGIN_HELLO_S2C_PACKET_ID);

    COMiC_Network_Buffer_WriteString(&buf, "", 20);
    COMiC_Network_Buffer_WriteRSAKey(&buf, getPublicKey());
    COMiC_Network_Buffer_WriteByteArray(&buf, nonce, 4);

    COMiC_Network_SendPacket(connection, &buf);
}
*/

    void sendLoginSuccessPacket(
            COMiC_In ClientNetInfo *connection
    )
    {
        Util::UUID uuid = Util::UUID::random();
        connection->uuid = &uuid;

        Buffer buf;

        buf.writePacketID(LOGIN_SUCCESS_S2C_PACKET_ID);

        char str[37];
        uuid.toString(str);
        buf.writeString(str, 36);
        buf.writeString(connection->username, 16);

        sendPacket(connection, &buf);

        connection->state = PLAY;
    }

    void sendGameJoinPacket(
            COMiC_In ClientNetInfo *connection
    )
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

    void sendHeldItemChangePacket(
            COMiC_In ClientNetInfo *connection
    )
    {
        Buffer buf;

        buf.writePacketID(HELD_ITEM_CHANGE_S2C_PACKET_ID);

        buf.write(4); // Selected hotbar shot

        sendPacket(connection, &buf);
    }
}