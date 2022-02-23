#include "main.h"

static COMiC_Util_RSA_Keypair keypair;

EVP_PKEY *getPublicKey()
{
    return keypair.publicKey;
}

int main(void)
{
    COMiC_Network_ServerNetInfo info;
    network_init(&info);

    COMiC_Util_RSA_generate_keypair(&keypair);

    COMiC_Network_ClientNetInfo client = (COMiC_Network_ClientNetInfo) {.state = HANDSHAKING};
    network_listen_to_connections(info, &client, COMiC_Network_handle_packet);

    network_cleanup(info);

    return 0;
}