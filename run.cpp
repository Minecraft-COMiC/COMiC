#include <iostream>
#include <COMiC/compression.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/network.hpp>

using namespace COMiC;

int main()
{
    if (Config::loadConfig())
    {
        std::cerr << "Failed to load configuration file. Aborting..." << std::endl;
        exit(1);
    }

    if (Crypto::init())
    {
        std::cerr << "Failed to initialize Crypto module. Aborting..." << std::endl;
        exit(1);
    }

    if (Compression::init())
    {
        std::cerr << "Failed to initialize Compression module. Aborting..." << std::endl;
        exit(1);
    }

    if (Network::init())
    {
        std::cerr << "Failed to initialize Network module. Aborting..." << std::endl;
        exit(1);
    }

    if (Network::listenToConnections())
    {
        std::cerr << "Network error. Aborting..." << std::endl;
    }

    if (Network::finalize())
    {
        std::cerr << "An error occurred whilst shutting down Network module" << std::endl;
        exit(1);
    }

//    std::string input =
//            R"(
//             O freddled gruntbuggly thy micturations are to me
//                 As plured gabbleblochits on a lurgid bee.
//              Groop, I implore thee my foonting turlingdromes.
//           And hooptiously drangle me with crinkly bindlewurdles,
//Or I will rend thee in the gobberwarts with my blurlecruncheon, see if I don't.
//
//                (by Prostetnic Vogon Jeltz; see p. 56/57)
//            )";
//    std::cout << input.length() << std::endl;
//    Compression::Deflater deflater;
//    std::string comp;
//    deflater.compress((Byte *) input.data(), input.length(), comp);
//    std::cout << comp.length() << std::endl;
//    for (const char &c: comp)
//        std::cout << (I32) c << ",";
//
//    std::cout << std::endl;

//    Compression::Inflater inflater;
//    char comp[] = {120, -100, 101, 80, 61, 79, -60, 48, 12, -35, -5, 43, -34, 6, 72, -43, 49, 29, 12, 76, 55, 33, 88, -114, -119, -67, 105, -36, 36, 34, -75, 43, 39, 81, 85, 126, 61, 110, 39, 56, 60, -40, -110, -29, -9, -111, 7, -4, -86, 43, 38, 37, -17, 51, 121, 4, 109, 92, 93, 11, 33, 111, -88, 113, -61, -100, -58, -38, 116, -88, 73, -72, 96, 80, 66, 21, -52, -44, -31, -74, 46, 5, 75, 110, -70, 51, 12, -50, 101, 114, 89, -58, -104, 106, -127, 48, 6, -40, 75, 72, 30, -114, -24, 116, 3, 125, 85, -111, -91, -57, 27, -46, -68, 100, -39, -7, 35, 17, -26, 13, -109, 8, -41, -60, 1, 38, -97, 109, 122, -107, -103, -54, 31, -8, -123, 61, -94, -63, -51, 92, 43, -26, -41, -21, -64, 33, 27, -102, -80, -90, 26, 49, 106, -30, 47, -37, -69, -60, -10, -73, -75, -87, -11, -46, 119, 87, 53, -71, 53, -27, 12, 37, 99, 56, 4, 19, -17, 19, 65, -100, 35, 93, 7, 53, -29, 7, -123, 25, 113, 102, 62, -45, 104, -71, -116, -111, -124, 123, -108, -3, 126, 50, 14, 47, 124, 87, 79, -35, -65, 48, -18, -35, -122, 15, -107, 82, -87, 114, 26, -15, 41, -63, 66, 120, -89, 92, -65, 95, 14, -16, 114, -62, -7, -23, -15, -4, -4, -16, 3, 43, 63, 122, 74};
//    std::string dec;
//    inflater.decompress((Byte *) comp, 243, dec);
//    std::cout << dec << std::endl;

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