#ifndef COMIC_CRYPTO_HPP
#define COMIC_CRYPTO_HPP

#include <COMiC/core.hpp>

namespace COMiC::Crypto
{
    class CryptoError : public Error
    {
    private:
        static std::string getOpenSSLError();

    protected:
        std::string message;

    public:
        explicit CryptoError(const char *msg)
        {
            auto openssl_err = getOpenSSLError();

            this->message = "COMiC::Crypto: ";
            this->message.append(msg);
            this->message.append(" (");
            if (openssl_err.empty())
                this->message.append("Unknown OpenSSL error");
            else
                this->message.append(openssl_err);
            this->message.append(")");
        }


        [[nodiscard]] const char *what() const noexcept override
        {
            return message.c_str();
        }
    };

    void init();

    void secureBytes(ByteVector &out);

    namespace Base64
    {
        [[nodiscard]] std::string encode(const std::string &input);

        [[nodiscard]] std::string decode(const std::string &input);
    }

    enum
    {
        MD5_LENGTH = 16,
        SHA1_LENGTH = 20,
        AES_KEY_SIZE = 16,
        RSA_MODULUS_BITS = 1024
    };

    class MD5
    {
        std::any md;

    public:
        MD5();

        void update(const std::string &str)
        {
            this->update((Byte *) str.data(), str.length());
        }

        void update(const ByteVector &vec)
        {
            this->update(vec.data(), vec.size());
        }

        void update(const Byte *input, USize len);

        ByteArray<MD5_LENGTH> final();

        static ByteArray<MD5_LENGTH> hash(const std::string &str);
    };

    class SHA1
    {
    private:
        std::any md;

    public:
        SHA1();

        void update(const std::string &str)
        {
            this->update((Byte *) str.data(), str.length());
        }

        void update(const ByteVector &vec)
        {
            this->update(vec.data(), vec.size());
        }

        void update(const Byte *input, USize len);

        ByteArray<SHA1_LENGTH> final();

        static ByteArray<SHA1_LENGTH> hash(const std::string &str);

        static std::string hexdigest(const Byte digest[SHA1_LENGTH]);
    };

    class AES
    {
    private:
        std::any encryptor, decryptor;

    public:
        AES() = default;

        void init(Byte key[AES_KEY_SIZE], Byte iv[AES_KEY_SIZE]);

        void encrypt(const Byte *in, USize len, Byte *out) const;

        void decrypt(const Byte *in, USize len, Byte *out) const;
    };

    class RSA
    {
    private:
        std::any cipher;
        ByteVector encodedPublicKey;

    public:
        explicit RSA(bool encrypting);

        [[nodiscard]] ByteVector encrypt(const ByteVector &in) const;

        [[nodiscard]] ByteVector decrypt(const ByteVector &in) const;

        [[nodiscard]] inline const ByteVector &getEncodedPublicKey() const
        {
            return this->encodedPublicKey;
        }
    };
}

#endif // COMIC_CRYPTO_HPP
