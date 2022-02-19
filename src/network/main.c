#include "main.h"

static Keypair keypair;

EVP_PKEY *getPublicKey()
{
    return keypair.publicKey;
}

int main(void)
{
    ServerNetInfo info;
    network_init(&info);

    util_rsa_generate_keypair(&keypair);

    ClientNetInfo client = (ClientNetInfo) {.state = HANDSHAKING};
    network_listen_to_connections(info, &client, network_handle_packet);

    network_cleanup(info);

    return 0;
}