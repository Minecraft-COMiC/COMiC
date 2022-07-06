#include "main.h"

int main(void)
{
    COMiC_Network_ServerNetInfo info;
    COMiC_Network_Init(&info);

    COMiC_Util_RSA_GenerateKeypair(&keypair);

    COMiC_Network_ClientNetInfo client = (COMiC_Network_ClientNetInfo) {.state = HANDSHAKING};
    COMiC_Network_ListenToConnections(info, &client, COMiC_Network_ReceivePacket);

    COMiC_Network_Finalize(info);

    return 0;
}