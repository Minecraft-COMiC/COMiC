#ifndef COMIC_RECEIVER_H
#define COMIC_RECEIVER_H

#include <stdio.h>
#include <COMiC/network.h>
#include "sender.h"
#include "../buffer.h"

void COMiC_Network_ReceivePacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_ByteBuffer *buf
);

#endif //COMIC_RECEIVER_H
