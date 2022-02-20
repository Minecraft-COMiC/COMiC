#ifndef COMIC_SENDER_H
#define COMIC_SENDER_H

#include <time.h>
#include <openssl/rand.h>
#include "../network.h"

void COMiC_Network_send_packet(COMiC_Network_ClientNetInfo *connection, COMiC_Network_ByteBuffer *buf);

void COMiC_Network_send_request_encryption_packet(COMiC_Network_ClientNetInfo *connection);

void COMiC_Network_send_login_success_packet(COMiC_Network_ClientNetInfo *connection);

void COMiC_Network_send_game_join_packet(COMiC_Network_ClientNetInfo *connection);

void COMiC_Network_send_held_item_change_packet(COMiC_Network_ClientNetInfo *connection);

#endif //COMIC_SENDER_H
