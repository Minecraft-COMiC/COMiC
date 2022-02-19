#ifndef COMIC_RECEIVER_H
#define COMIC_RECEIVER_H

#include <stdio.h>
#include "sender.h"
#include "../network.h"

void network_handle_packet(ClientNetInfo *connection, ByteBuffer *buf);

#endif //COMIC_RECEIVER_H
