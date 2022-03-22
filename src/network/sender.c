#include <COMiC/network.h>

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

void COMiC_Network_SendLoginSuccessPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
)
{
    COMiC_Util_JUUID uuid = COMiC_Util_JUUID_InitRandom();
    connection->uuid = uuid;

    COMiC_Network_Buffer buf = COMiC_Network_Buffer_InitWithDefaultCapacity();

    COMiC_Network_Buffer_WritePacketID(&buf, LOGIN_SUCCESS_S2C_PACKET_ID);

    char str[37];
    COMiC_Util_JUUID_ToString(uuid, str);
    COMiC_Network_Buffer_WriteString(&buf, str, 36);
    COMiC_Network_Buffer_WriteString(&buf, connection->username, 16);

    COMiC_Network_SendPacket(connection, &buf);

    connection->state = PLAY;
}

void COMiC_Network_SendGameJoinPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
)
{
    COMiC_Network_Buffer buf = COMiC_Network_Buffer_InitWithDefaultCapacity();

    COMiC_Network_Buffer_WritePacketID(&buf, GAME_JOIN_S2C_PACKET_ID);

    COMiC_Network_Buffer_WriteInt(&buf, 0); // Player Entitiy ID
    COMiC_Network_Buffer_WriteByte(&buf, 0); // Gamemode, no hardcore
    COMiC_Network_Buffer_WriteInt(&buf, 0); // Overworld raw id
    COMiC_Network_Buffer_WriteByte(&buf, 20); // Max players
    COMiC_Network_Buffer_WriteString(&buf, "flat", 16); // Generator type
    COMiC_Network_Buffer_WriteVarInt(&buf, 10); // View distance
    COMiC_Network_Buffer_WriteBool(&buf, 0); // Show debug info

    COMiC_Network_SendPacket(connection, &buf);
}

void COMiC_Network_SendHeldItemChangePacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
)
{
    COMiC_Network_Buffer buf = COMiC_Network_Buffer_InitWithDefaultCapacity();

    COMiC_Network_Buffer_WritePacketID(&buf, HELD_ITEM_CHANGE_S2C_PACKET_ID);

    COMiC_Network_Buffer_WriteByte(&buf, 4); // Selected hotbar shot

    COMiC_Network_SendPacket(connection, &buf);
}
