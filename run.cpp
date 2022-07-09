#include <iostream>
#include <COMiC/network.hpp>

using namespace COMiC;

int main()
{
//    COMiC::Network::ServerNetInfo info{};
//    COMiC::Network::init(&info);
//
////    COMiC::Util::RSA::generateKeypair(&keypair);
//
//    COMiC::Network::ClientNetInfo client{.state = COMiC::Network::NetworkState::HANDSHAKING, .uuid = nullptr};
//    COMiC::Network::listenToConnections(info, &client);
//
//    COMiC::Network::finalize(info);

    Byte buf[1024];
    size_t written;
    Network::sendHTTPGet("sessionserver.mojang.com", "", buf, &written);
    puts(reinterpret_cast<const char *>(buf));

    return 0;
}