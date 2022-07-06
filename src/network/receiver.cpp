#include "COMiC/network.hpp"

namespace COMiC::Network
{
    void receivePacket(
            COMiC_In ClientNetInfo *connection,
            COMiC_In Buffer *buf
    )
    {
        PacketID packet_id = buf->readPacketID(connection->state);

        puts("/////////////////NEW PACKET/////////////////");
        printf("Id: 0x%s%X\n", (packet_id & 0xFF) < 0x10 ? "0" : "", packet_id & 0xFF);
        printf("Network State: %d\n", connection->state);

        switch (packet_id)
        {
            case HANDSHAKE_C2S_PACKET_ID:
                printf("Protocol Version: %d\n", buf->readVarInt());

                char address[255];
                buf->readString(address, 255);
                printf("Server Address: %s\n", address);

                printf("Server Port: %d\n", buf->readShort());
                printf("New State: %d\n", connection->state = (NetworkState) buf->readEnum());

                break;
            case HELD_ITEM_CHANGE_S2C_PACKET_ID:
                sendHeldItemChangePacket(connection);
                puts("Held Item Change packet sent");

                break;
            case LOGIN_HELLO_C2S_PACKET_ID:
                connection->username = static_cast<char *>(malloc(16));
                buf->readString(connection->username, 16);
                printf("Username: %s\n", connection->username);

//                COMiC_Network_send_request_encryption_packet(connection); // For now we are going to stay in offline mode
                sendLoginSuccessPacket(connection);
                puts("Login Success packet sent");

                sendGameJoinPacket(connection);
                puts("Join Game packet sent");

                break;
            default:
                break;
        }

        printf("Bytes = [");
        for (I32 i = 0; i < buf->getSize(); i++)
            printf("%d%s", (U8) buf->getBytes()[i], i != buf->getSize() - 1 ? ", " : "]\n");
    }
}