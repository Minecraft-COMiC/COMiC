#define OPENSSL_API_COMPAT 30000 // We're not supporting old (< 3.0) OpenSSL versions
#define OPENSSL_NO_DEPRECATED // Resolve name conflicts (RSA, MD5, SHA1)

#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include <COMiC/crypto.hpp>

#define MD std::any_cast<std::shared_ptr<EVP_MD_CTX>&>(this->md)

namespace COMiC::Crypto
{
    MD5::MD5() : md(std::make_any<std::shared_ptr<EVP_MD_CTX>>(EVP_MD_CTX_new(), EVP_MD_CTX_free))
    {
        if (MD == nullptr)
            throw CryptoError("Could not create context for MD5");

        if (EVP_DigestInit_ex(MD.get(), EVP_md5(), nullptr) != 1)
            throw CryptoError("Failed to initialize MD5 digest");
    }

    void MD5::update(const Byte *input, USize len)
    {
        if (EVP_DigestUpdate(MD.get(), input, len) != 1)
            throw CryptoError("MD5#update() failed");
    }

    ByteArray<MD5_LENGTH> MD5::final()
    {
        ByteArray<MD5_LENGTH> out{};
        if (EVP_DigestFinal(MD.get(), out.data(), nullptr) != 1)
            throw CryptoError("MD5#final() failed");

        return out;
    }

    ByteArray<MD5_LENGTH> MD5::hash(const std::string &str)
    {
        ByteArray<MD5_LENGTH> out{};
        if (EVP_Digest(str.c_str(), str.length(), out.data(), nullptr, EVP_md5(), nullptr) != 1)
            throw CryptoError("MD5#hash() failed");

        return out;
    }

    SHA1::SHA1() : md(std::make_any<std::shared_ptr<EVP_MD_CTX>>(EVP_MD_CTX_new(), EVP_MD_CTX_free))
    {
        if (MD == nullptr)
            throw CryptoError("Could not create context for SHA-1");

        if (EVP_DigestInit_ex(MD.get(), EVP_sha1(), nullptr) != 1)
            throw CryptoError("Failed to initialize SHA-1 digest");
    }

    void SHA1::update(const Byte *input, USize len)
    {
        if (EVP_DigestUpdate(MD.get(), input, len) != 1)
            throw CryptoError("SHA1#update() failed");
    }

    ByteArray<SHA1_LENGTH> SHA1::final()
    {
        ByteArray<SHA1_LENGTH> out{};
        if (EVP_DigestFinal(MD.get(), out.data(), nullptr) != 1)
            throw CryptoError("SHA1#final() failed");

        return out;
    }

    ByteArray<SHA1_LENGTH> SHA1::hash(const std::string &str)
    {
        ByteArray<SHA1_LENGTH> out{};
        if (EVP_Digest(str.c_str(), str.length(), out.data(), nullptr, EVP_sha1(), nullptr) != 1)
            throw CryptoError("SHA1#hash() failed");

        return out;
    }

    std::string SHA1::hexdigest(const Byte digest[SHA1_LENGTH])
    {
        Byte copy[SHA1_LENGTH];
        memcpy(copy, digest, SHA1_LENGTH);

        std::shared_ptr<BIGNUM> bn(BN_new(), BN_free);
        if (bn == nullptr)
            throw CryptoError("SHA1#hexdigest() failed");

        if (((I8) copy[0]) < 0)
        {
            for (Byte &b: copy)
                b = ~b;

            if (BN_bin2bn(copy, SHA1_LENGTH, bn.get()) == nullptr || BN_add_word(bn.get(), 1) != 1)
                throw CryptoError("SHA1#hexdigest() failed");

            BN_set_negative(bn.get(), true);
        }
        else
        {
            if (BN_bin2bn(copy, SHA1_LENGTH, bn.get()) == nullptr)
                throw CryptoError("SHA1#hexdigest() failed");
        }

        std::shared_ptr<char> ans(BN_bn2hex(bn.get()), [](char *ptr) { OPENSSL_free(ptr); });
        USize size;

        if (ans == nullptr)
            throw CryptoError("SHA1#hexdigest() failed");
        else
            size = strlen(ans.get()) + 1;

        // Convert ans[] to lowercase:
        for (I32 i = 0; i < size; i++)
            ans.get()[i] = (char) tolower(ans.get()[i]);

        // Delete leading zero in ans[] to avoid -01 or 05:
        U8 offset;
        std::string out;
        if (ans.get()[0] == '-')
        {
            out = '-';
            offset = 1;
        }
        else offset = 0;

        if (ans.get()[offset] == '0')
            out.append(ans.get() + offset + 1, size - offset - 1);
        else
            out.append(ans.get(), size);

        return out;
    }
}