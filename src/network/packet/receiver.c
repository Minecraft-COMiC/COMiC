#include "receiver.h"

void COMiC_Network_ReceivePacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_ByteBuffer *buf
)
{
    int packet_id = COMiC_Network_Buffer_ReadPacketID(buf);

    puts("/////////////////NEW PACKET/////////////////");
    printf("Id: 0x%s%X\n", packet_id < 0x10 ? "0" : "", packet_id);
    printf("Network State: %d\n", connection->state);

    packet_id |= (connection->state + 1) << 8;

    switch (packet_id)
    {
        case HANDSHAKE_C2S_PACKET_ID:
            printf("Protocol Version: %d\n", COMiC_Network_Buffer_ReadVarInt(buf));

            char address[255];
            COMiC_Network_Buffer_ReadString(buf, address, 255);
            printf("Server Address: %s\n", address);

            printf("Server Port: %d\n", COMiC_Network_Buffer_ReadShort(buf));
            printf("New State: %d\n", connection->state = COMiC_Network_Buffer_ReadEnum(buf));

            break;
        case HELD_ITEM_CHANGE_S2C_PACKET_ID:
            COMiC_Network_SendHeldItemChangePacket(connection);
            puts("Held Item Change packet sent");

            break;
        case LOGIN_HELLO_C2S_PACKET_ID:
            connection->username = malloc(16);
            COMiC_Network_Buffer_ReadString(buf, connection->username, 16);
            printf("Username: %s\n", connection->username);

//                COMiC_Network_send_request_encryption_packet(connection); // For now we are going to stay in offline mode
            COMiC_Network_SendLoginSuccessPacket(connection);
            puts("Login Success packet sent");

            COMiC_Network_SendGameJoinPacket(connection);
            puts("Join Game packet sent");

            break;
        default:
            break;
    }

    printf("Bytes = [");
    for (int i = 0; i < buf->size; i++)
        printf("%d%s", (unsigned char) buf->bytes[i], i != buf->size - 1 ? ", " : "]\n");
}