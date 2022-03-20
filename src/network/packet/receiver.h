#ifndef COMIC_RECEIVER_H
#define COMIC_RECEIVER_H

#include <stdio.h>
#include "sender.h"
#include "../network.h"

void COMiC_Network_handle_packet(COMiC_Network_ClientNetInfo *connection, COMiC_Network_ByteBuffer *buf);

#endif //COMIC_RECEIVER_H
