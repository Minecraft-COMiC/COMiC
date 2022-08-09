#define OPENSSL_API_COMPAT 30000 // We're not supporting old (< 3.0) OpenSSL versions
#define OPENSSL_NO_DEPRECATED // Resolve name conflicts (RSA, MD5, SHA1)

#include <openssl/aes.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

#include <COMiC/crypto.hpp>

namespace COMiC::Crypto
{
#define ENCRYPTOR std::any_cast<std::shared_ptr<EVP_CIPHER_CTX>>(this->encryptor)
#define DECRYPTOR std::any_cast<std::shared_ptr<EVP_CIPHER_CTX>>(this->decryptor)

    void AES::init(Byte key[AES_KEY_SIZE], Byte iv[AES_KEY_SIZE])
    {
        this->encryptor = std::make_any<std::shared_ptr<EVP_CIPHER_CTX>>(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
        if (ENCRYPTOR == nullptr)
            throw CryptoError("Could not create encryption cipher context for AES");

        this->decryptor = std::make_any<std::shared_ptr<EVP_CIPHER_CTX>>(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
        if (DECRYPTOR == nullptr)
            throw CryptoError("Could not create decryption cipher context for AES");

        if (EVP_EncryptInit(ENCRYPTOR.get(), EVP_aes_128_cfb8(), key, iv) != 1)
            throw CryptoError("AES encryption initialization failed");

        if (EVP_DecryptInit(DECRYPTOR.get(), EVP_aes_128_cfb8(), key, iv) != 1)
            throw CryptoError("AES decryption initialization failed");
    }

    void AES::encrypt(const Byte *in, USize len, Byte *out) const
    {
        int truncated = (int) len;
        if (EVP_EncryptUpdate(ENCRYPTOR.get(), out, &truncated, in, truncated) != 1)
            throw CryptoError("AES failed to encrypt");
    }

    void AES::decrypt(const Byte *in, USize len, Byte *out) const
    {
        int truncated = (int) len;
        if (EVP_DecryptUpdate(DECRYPTOR.get(), out, &truncated, in, truncated) != 1)
            throw CryptoError("AES failed to decrypt");
    }

#define CIPHER std::any_cast<std::shared_ptr<EVP_PKEY_CTX>>(this->cipher)

    EVP_PKEY *generateKeyPair()
    {
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        EVP_PKEY *pkey = nullptr;

        if (ctx == nullptr)
            goto ERR;

        if (EVP_PKEY_keygen_init(ctx) != 1 ||
            EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, RSA_MODULUS_BITS) != 1 ||
            EVP_PKEY_keygen(ctx, &pkey) != 1)
            goto ERR;

        EVP_PKEY_CTX_free(ctx);

        return pkey;

        ERR:
        if (ctx != nullptr)
            EVP_PKEY_CTX_free(ctx);

        if (pkey != nullptr)
            EVP_PKEY_free(pkey);

        throw CryptoError("Failed to generate RSA keypair");
    }

    ByteVector encodePublicKey(EVP_PKEY *key)
    {
        I32 len = i2d_PUBKEY(key, nullptr);
        if (len <= 0)
            throw CryptoError("Failed to encode RSA public key");

        std::shared_ptr<Byte> tmp((Byte *) OPENSSL_malloc(len), [](Byte *ptr) { OPENSSL_free(ptr); });
        if (tmp == nullptr)
            throw CryptoError("Failed to encode RSA public key");

        auto out = tmp.get();
        if (i2d_PUBKEY(key, &out) != len)
            throw CryptoError("Failed to encode RSA public key");

        return {tmp.get(), tmp.get() + len};
    }

    RSA::RSA(bool encrypting)
    {
        auto keypair = generateKeyPair();
        this->encodedPublicKey = encodePublicKey(keypair);

        this->cipher = std::make_any<std::shared_ptr<EVP_PKEY_CTX>>(
            EVP_PKEY_CTX_new(keypair, nullptr),
            EVP_PKEY_CTX_free
        );
        if (CIPHER == nullptr)
            throw CryptoError("Could not create cipher context for RSA");

        if (encrypting)
        {
            if (EVP_PKEY_encrypt_init(CIPHER.get()) != 1)
                throw CryptoError("RSA encryption initialization failed");
        }
        else
        {
            if (EVP_PKEY_decrypt_init(CIPHER.get()) != 1)
                throw CryptoError("RSA decryption initialization failed");
        }

        if (EVP_PKEY_CTX_set_rsa_padding(CIPHER.get(), RSA_PKCS1_PADDING) != 1)
            throw CryptoError("Unable to set RSA padding");
    }

    [[nodiscard]] ByteVector RSA::encrypt(const ByteVector &in) const
    {
        ByteVector out(RSA_MODULUS_BITS / BYTE_SIZE);

        size_t written;
        if (EVP_PKEY_encrypt(CIPHER.get(), out.data(), &written, in.data(), in.size()) != 1)
            throw CryptoError("RSA failed to encrypt");

        return out;
    }

    [[nodiscard]] ByteVector RSA::decrypt(const ByteVector &in) const
    {
        ByteVector out(RSA_MODULUS_BITS / BYTE_SIZE);

        size_t written;
        if (EVP_PKEY_decrypt(CIPHER.get(), out.data(), &written, in.data(), in.size()) != 1)
            throw CryptoError("RSA failed to decrypt");

        out.resize(written);

        return out;
    }
}