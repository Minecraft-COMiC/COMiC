#include "sender.h"

void network_send_request_encryption_packet(ClientNetInfo *connection)
{
    char nonce[4];
    RAND_bytes((unsigned char *) nonce, 4);

    ByteBuffer buf = network_buffer_new_with_default_capacity();

    network_buffer_write_packet_id(&buf, LOGIN_HELLO_S2C_PACKET_ID);

    network_buffer_write_string(&buf, "", 20);
    network_buffer_write_rsa_key(&buf, getPublicKey());
    network_buffer_write_byte_array(&buf, nonce, 4);

    network_send_packet(connection, &buf);
}

void network_send_login_success_packet(ClientNetInfo *connection)
{
    JUUID uuid = util_uuid_random();
    connection->uuid = uuid;

    ByteBuffer buf = network_buffer_new_with_default_capacity();

    network_buffer_write_packet_id(&buf, LOGIN_SUCCESS_S2C_PACKET_ID);

    char str[37];
    util_uuid_to_string(uuid, str);
    network_buffer_write_string(&buf, str, 36);
    network_buffer_write_string(&buf, connection->username, 16);

    network_send_packet(connection, &buf);

    connection->state = PLAY;
}

void network_send_game_join_packet(ClientNetInfo *connection)
{
    ByteBuffer buf = network_buffer_new_with_default_capacity();

    network_buffer_write_packet_id(&buf, GAME_JOIN_S2C_PACKET_ID);

    network_buffer_write_int(&buf, 0); // Player Entitiy ID
    network_buffer_write_byte(&buf, 0); // Gamemode, no hardcore
    network_buffer_write_int(&buf, 0); // Overworld raw id
    network_buffer_write_byte(&buf, 20); // Max players
    network_buffer_write_string(&buf, "flat", 16); // Generator type
    network_buffer_write_var_int(&buf, 10); // View distance
    network_buffer_write_bool(&buf, 0); // Show debug info

    network_send_packet(connection, &buf);
}

void network_send_held_item_change_packet(ClientNetInfo *connection)
{
    ByteBuffer buf = network_buffer_new_with_default_capacity();

    network_buffer_write_packet_id(&buf, HELD_ITEM_CHANGE_S2C_PACKET_ID);

    network_buffer_write_byte(&buf, 4); // Selected hotbar shot

    network_send_packet(connection, &buf);
}
