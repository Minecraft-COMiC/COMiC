#include <iostream>
#include <COMiC/crypto.hpp>
#include <COMiC/network.hpp>

using namespace COMiC;

int main()
{
    Network::NetManager server{};
    Network::init(&server);

    Crypto::init();

    server.rsa = new Crypto::RSA(false);

    Network::ClientNetInfo client{};
    client.state = Network::NetworkState::HANDSHAKING;
    Network::listenToConnections(server, &client);

    Network::finalize(server);

//    Byte buf[2048];
//    size_t written;
//    Network::sendHTTPGet("sessionserver.mojang.com", "/", buf, &written);
//    puts((char *)(buf));

//    char encoded[] = {-5, -49, 125, 88, 48, 106, 20, 110, 109, 76, -101, 93, 77, -125, -5, 29};
//    Crypto::AES enc(reinterpret_cast<Byte *>(encoded), reinterpret_cast<Byte *>(encoded));
//
//    Byte str[] = "Veni, vidi, vici.";
//    Byte str2[] = "Per aspera ad astra";
//    Byte *enc = new Byte[32 + strlen((char *) str2)];
//
//    std::cout << strlen((char *) str2) << std::endl;
//    enc.encrypt(str, strlen((char *) str) + 1, enc);
//    enc.encrypt(str2, strlen((char *) str2) + 1, enc);
//
//    puts("-----------------");
//    for (size_t i = 0; i < strlen((char *) str2) + 1; i++)
//        printf("%d, ", (char) enc[i]);

//    char enc[64];
//    Byte digest[20];
//    Crypto::SHA1::hash("jeb_", digest);


//    Crypto::SHA1::hash("jeb_", digest);
//    Crypto::SHA1::hexdigest(digest, enc);
//    puts(enc);

//    Crypto::RSA::KeyPair *keypair = nullptr;
//    Crypto::RSA::generateKeyPair(&keypair);

//    USize size;
//    Crypto::RSA::encodePublicKey(nullptr, nullptr, &size);

//    BIGNUM *n = BN_new(), *e = BN_new(), *d = BN_new(), *p = BN_new(), *q = BN_new();
//    BN_dec2bn(&n, "116640083578504542887582041278753177820595384191866274197759231472578080983259720459719737494267208782315581866897889829296964202566117853844596640062921616708114940906286229966694867221656984008668605158004523811792947617230209660272972177826479542865319141736727666770586004065356486604944964850960435817053");
//    BN_dec2bn(&e, "65537");
//    BN_dec2bn(&d, "34507752574936670614268706201897330740856674917925678478116998627062825765360372308673054767174983766121684190873600956715120297290911379345909703742618472074424330810595886927863781064646023668507555277432444031347302062248491277288993747427273170683350462207237602006253737240763233734758831438182095974253");
//    BN_dec2bn(&p, "12222405528865504256237402592649531980162716141621343394963299625333595600360500781860586094947157895377578090641245781478837287512343992168503047816422343");
//    BN_dec2bn(&q, "9543136439307065812937533616589056952210335379096148978429059203182413330593305184116552303632941177993790091421177811324054831569068581372552763822454971");
//
//    OSSL_PARAM_BLD *params_build = OSSL_PARAM_BLD_new();
//
//    OSSL_PARAM_BLD_push_BN(params_build, "n", n);
//    OSSL_PARAM_BLD_push_BN(params_build, "e", e);
//    OSSL_PARAM_BLD_push_BN(params_build, "d", d);
//    OSSL_PARAM_BLD_push_BN(params_build, "p", p);
//    OSSL_PARAM_BLD_push_BN(params_build, "q", q);
//    OSSL_PARAM *params = OSSL_PARAM_BLD_to_param(params_build);
//
//    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_name(nullptr, "RSA", nullptr);
//    EVP_PKEY *pkey = nullptr;
//
//    EVP_PKEY_fromdata_init(ctx);
//    EVP_PKEY_fromdata(ctx, &pkey, EVP_PKEY_KEYPAIR, params);
//    EVP_PKEY_CTX_free(ctx);
//
//    Byte enc[128];
//    Byte dec[20];
//    USize written;
//    Crypto::RSA rsa(pkey, true);
//    rsa.encrypt(str, strlen((char *) str), enc, &written);
//    rsa.decrypt(enc, 128, dec, &written);
//
//    printf("%d\n", written);
//    puts("-----------------");
//    for (size_t i = 0; i < written; i++)
//        printf("%c", (char) dec[i]);
//
    return 0;
}