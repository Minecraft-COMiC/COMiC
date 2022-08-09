#define OPENSSL_API_COMPAT 30000 // We're not supporting old (< 3.0) OpenSSL versions
#define OPENSSL_NO_DEPRECATED // Resolve name conflicts (RSA, MD5, SHA1)

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <COMiC/crypto.hpp>

namespace COMiC::Crypto
{
    std::string CryptoError::getOpenSSLError()
    {
        char buf[256];

        ERR_error_string_n(ERR_get_error(), buf, sizeof(buf));

        return {buf};
    }

    void init()
    {
        if (OPENSSL_init_crypto(
                OPENSSL_INIT_NO_ADD_ALL_CIPHERS |
                OPENSSL_INIT_NO_ADD_ALL_DIGESTS,
                nullptr
        ) != 1)
            throw CryptoError("OpenSSL initialization failed");

        if (EVP_add_cipher(EVP_aes_128_cfb8()) != 1)
            throw CryptoError("Failed to add AES CFB8 cipher");

        if (EVP_add_digest(EVP_sha1()) != 1)
            throw CryptoError("Failed to add SHA1 digest");

        if (EVP_add_digest(EVP_md5()) != 1)
            throw CryptoError("Failed to add MD5 digest");

        std::cout << "Using `" OPENSSL_VERSION_TEXT "` as COMiC::Crypto API" << std::endl;
    }

    void secureBytes(ByteVector &out)
    {
        if (RAND_bytes(out.data(), (int) out.capacity()) != 1)
            throw CryptoError("secureBytes() failed");
    }

    namespace Base64
    {
        [[nodiscard]] std::string encode(const std::string &input)
        {
            std::string out(4 * ((input.size() + 2) / 3) + 1, 0);
            if (EVP_EncodeBlock((Byte *) out.data(), (Byte *) input.data(), (int) input.size()) != out.capacity() - 1)
                throw CryptoError("Base64#encode() failed");

            return out;
        }

        [[nodiscard]] std::string decode(const std::string &input)
        {
            std::string out(3 * input.size() / 4, 0);
            if (EVP_DecodeBlock((Byte *) out.data(), (Byte *) input.data(), (int) input.size()) != out.capacity())
                throw CryptoError("Base64#decode() failed");

            return out;
        }
    }
}