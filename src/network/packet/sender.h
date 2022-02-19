#ifndef COMIC_SENDER_H
#define COMIC_SENDER_H

#include <time.h>
#include <openssl/rand.h>
#include "../network.h"

void network_send_packet(ClientNetInfo *connection, ByteBuffer *buf);

void network_send_request_encryption_packet(ClientNetInfo *connection);

void network_send_login_success_packet(ClientNetInfo *connection);

void network_send_game_join_packet(ClientNetInfo *connection);

void network_send_held_item_change_packet(ClientNetInfo *connection);

#endif //COMIC_SENDER_H
