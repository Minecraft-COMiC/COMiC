#ifndef COMIC_SENDER_H
#define COMIC_SENDER_H

#include <time.h>
#include <openssl/rand.h>
#include <COMiC/network.h>
#include "../buffer.h"

void COMiC_Network_SendPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_ByteBuffer *buf
);

/*
void COMiC_Network_SendRequestEncryptionPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);
*/

void COMiC_Network_SendLoginSuccessPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);

void COMiC_Network_SendGameJoinPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);

void COMiC_Network_SendHeldItemChangePacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);

#endif //COMIC_SENDER_H
