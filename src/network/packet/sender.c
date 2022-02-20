#include "sender.h"

void COMiC_Network_send_request_encryption_packet(COMiC_Network_ClientNetInfo *connection)
{
    char nonce[4];
    RAND_bytes((unsigned char *) nonce, 4);

    COMiC_Network_ByteBuffer buf = COMiC_Network_Buffer_new_with_default_capacity();

    COMiC_Network_Buffer_write_packet_id(&buf, LOGIN_HELLO_S2C_PACKET_ID);

    COMiC_Network_Buffer_write_string(&buf, "", 20);
    COMiC_Network_Buffer_write_rsa_key(&buf, getPublicKey());
    COMiC_Network_Buffer_write_byte_array(&buf, nonce, 4);

    COMiC_Network_send_packet(connection, &buf);
}

void COMiC_Network_send_login_success_packet(COMiC_Network_ClientNetInfo *connection)
{
    COMiC_Util_JUUID uuid = COMiC_Util_JUUID_random();
    connection->uuid = uuid;

    COMiC_Network_ByteBuffer buf = COMiC_Network_Buffer_new_with_default_capacity();

    COMiC_Network_Buffer_write_packet_id(&buf, LOGIN_SUCCESS_S2C_PACKET_ID);

    char str[37];
    COMiC_Util_JUUID_to_string(uuid, str);
    COMiC_Network_Buffer_write_string(&buf, str, 36);
    COMiC_Network_Buffer_write_string(&buf, connection->username, 16);

    COMiC_Network_send_packet(connection, &buf);

    connection->state = PLAY;
}

void COMiC_Network_send_game_join_packet(COMiC_Network_ClientNetInfo *connection)
{
    COMiC_Network_ByteBuffer buf = COMiC_Network_Buffer_new_with_default_capacity();

    COMiC_Network_Buffer_write_packet_id(&buf, GAME_JOIN_S2C_PACKET_ID);

    COMiC_Network_Buffer_write_int(&buf, 0); // Player Entitiy ID
    COMiC_Network_Buffer_write_byte(&buf, 0); // Gamemode, no hardcore
    COMiC_Network_Buffer_write_int(&buf, 0); // Overworld raw id
    COMiC_Network_Buffer_write_byte(&buf, 20); // Max players
    COMiC_Network_Buffer_write_string(&buf, "flat", 16); // Generator type
    COMiC_Network_Buffer_write_var_int(&buf, 10); // View distance
    COMiC_network_buffer_write_bool(&buf, 0); // Show debug info

    COMiC_Network_send_packet(connection, &buf);
}

void COMiC_Network_send_held_item_change_packet(COMiC_Network_ClientNetInfo *connection)
{
    COMiC_Network_ByteBuffer buf = COMiC_Network_Buffer_new_with_default_capacity();

    COMiC_Network_Buffer_write_packet_id(&buf, HELD_ITEM_CHANGE_S2C_PACKET_ID);

    COMiC_Network_Buffer_write_byte(&buf, 4); // Selected hotbar shot

    COMiC_Network_send_packet(connection, &buf);
}
