#ifndef COMIC_CRYPTO_HPP
#define COMIC_CRYPTO_HPP

#include <iostream>
#include <cctype>
#include <cstring>

#define OPENSSL_API_COMPAT 30000 // We're not supporting old (< 3.0) OpenSSL versions
#define OPENSSL_NO_DEPRECATED // Resolve name conflicts (RSA, MD5, SHA1)

#include <openssl/aes.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/opensslv.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#include <COMiC/core.hpp>

namespace COMiC::Crypto
{
    // For internal use only
    static inline void _print_error(const std::string &msg)
    {
        char str[256];
        ERR_error_string_n(ERR_get_error(), str, sizeof(str));
        std::cerr << msg << str << std::endl;
    }

    static inline IfError init()
    {
        if (OPENSSL_init_crypto(
                OPENSSL_INIT_NO_ADD_ALL_CIPHERS |
                OPENSSL_INIT_NO_ADD_ALL_DIGESTS,
                nullptr
        ) != 1)
        {
            _print_error("OpenSSL initialization failed: ");
            return FAIL;
        }

        if (EVP_add_cipher(EVP_aes_128_cfb8()) != 1)
        {
            _print_error("Failed to add AES CFB8 cipher: ");
            return FAIL;
        }

        if (EVP_add_digest(EVP_sha1()) != 1)
        {
            _print_error("Failed to add SHA1 digest: ");
            return FAIL;
        }

        if (EVP_add_digest(EVP_md5()) != 1)
        {
            _print_error("Failed to add MD5 digest: ");
            return FAIL;
        }

        std::cout << "Using `" OPENSSL_VERSION_TEXT "` as COMiC::Crypto API" << std::endl;

        return SUCCESS;
    }

    static inline IfError secureBytes(Byte *out, USize len)
    {
        if (RAND_bytes(out, (int) len) != 1)
        {
            _print_error("secureBytes() failed: ");
            return FAIL;
        }

        return SUCCESS;
    }

    class MD5
    {
        EVP_MD_CTX *ctx = nullptr;

    public:
        MD5() = default;

        IfError init()
        {
            this->ctx = EVP_MD_CTX_new();
            if (this->ctx == nullptr)
            {
                _print_error("Could not create context for MD5: ");
                return FAIL;
            }

            if (EVP_DigestInit_ex(this->ctx, EVP_md5(), nullptr) != 1)
            {
                _print_error("MD5 constructor failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError update(const Byte *input, USize len)
        {
            if (EVP_DigestUpdate(this->ctx, input, len) != 1)
            {
                _print_error("MD5#update() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError final(Byte out[MD5_DIGEST_LENGTH])
        {
            if (EVP_DigestFinal(this->ctx, out, nullptr) != 1)
            {
                _print_error("MD5#final() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        static IfError hash(const std::string &str, Byte out[MD5_DIGEST_LENGTH])
        {
            if (EVP_Digest(str.c_str(), str.length(), out, nullptr, EVP_md5(), nullptr) != 1)
            {
                _print_error("MD5#hash() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }


        ~MD5()
        {
            if (this->ctx != nullptr)
                EVP_MD_CTX_free(this->ctx);
        }
    };

    class SHA1
    {
    private:
        EVP_MD_CTX *ctx = nullptr;

    public:
        SHA1() = default;

        IfError init()
        {
            this->ctx = EVP_MD_CTX_new();
            if (this->ctx == nullptr)
            {
                _print_error("Could not create context for SHA1: ");
                return FAIL;
            }

            if (EVP_DigestInit_ex(this->ctx, EVP_sha1(), nullptr) != 1)
            {
                _print_error("SHA1 constructor failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError update(const Byte *input, USize len)
        {
            if (EVP_DigestUpdate(this->ctx, input, len) != 1)
            {
                _print_error("SHA1#update() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError final(Byte out[SHA_DIGEST_LENGTH])
        {
            if (EVP_DigestFinal(this->ctx, out, nullptr) != 1)
            {
                _print_error("SHA1#final() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        static IfError hash(Byte out[SHA_DIGEST_LENGTH], const std::string &str)
        {
            if (EVP_Digest(str.c_str(), str.length(), out, nullptr, EVP_sha1(), nullptr) != 1)
            {
                _print_error("SHA1#hash() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        static IfError hexdigest(const Byte digest[SHA_DIGEST_LENGTH], std::string &out)
        {
            Byte copy[SHA_DIGEST_LENGTH];
            memcpy(copy, digest, SHA_DIGEST_LENGTH);

            BIGNUM *bn = BN_new();
            if (bn == nullptr)
            {
                _print_error("SHA1#hexdigest() failed: ");
                return FAIL;
            }

            if (((I8) copy[0]) < 0)
            {
                for (Byte &b: copy)
                    b = ~b;

                if (BN_bin2bn(copy, SHA_DIGEST_LENGTH, bn) == nullptr ||
                    BN_add_word(bn, 1) != 1)
                {
                    _print_error("SHA1#hexdigest() failed: ");
                    BN_free(bn);
                    return FAIL;
                }

                BN_set_negative(bn, true);
            } else
            {
                if (BN_bin2bn(copy, SHA_DIGEST_LENGTH, bn) == nullptr)
                {
                    _print_error("SHA1#hexdigest() failed: ");
                    BN_free(bn);
                    return FAIL;
                }
            }

            char *ans;
            USize size;

            ans = BN_bn2hex(bn);
            if (ans == nullptr)
            {
                _print_error("SHA1#hexdigest() failed: ");
                BN_free(bn);
                return FAIL;
            } else
                size = strlen(ans) + 1;

            // Convert ans[] to lowercase:
            for (I32 i = 0; i < size; i++)
                ans[i] = (char) tolower(ans[i]);

            // Delete leading zero in ans[] to avoid -01 or 05:
            U8 offset;
            if (ans[0] == '-')
            {
                out = '-';
                offset = 1;
            } else offset = 0;

            if (ans[offset] == '0')
                out.append(ans + offset + 1, size - offset - 1);
            else
                out.append(ans, size);

            BN_free(bn);
            OPENSSL_free(ans);

            return SUCCESS;
        }

        ~SHA1()
        {
            if (this->ctx != nullptr)
                EVP_MD_CTX_free(this->ctx);
        }
    };

    class AES
    {
    private:
        EVP_CIPHER_CTX *encryptor = nullptr, *decryptor = nullptr;

    public:
        AES() = default;

        IfError init(Byte key[AES_BLOCK_SIZE], Byte iv[AES_BLOCK_SIZE])
        {
            this->encryptor = EVP_CIPHER_CTX_new();
            if (this->encryptor == nullptr)
            {
                _print_error("Could not create encryption cipher context for AES: ");
                return FAIL;
            }

            this->decryptor = EVP_CIPHER_CTX_new();
            if (this->decryptor == nullptr)
            {
                _print_error("Could not create decryption cipher context for AES: ");
                return FAIL;
            }

            if (EVP_EncryptInit(this->encryptor, EVP_aes_128_cfb8(), key, iv) != 1)
            {
                _print_error("AES encryption initialization failed: ");
                return FAIL;
            }

            if (EVP_DecryptInit(this->decryptor, EVP_aes_128_cfb8(), key, iv) != 1)
            {
                _print_error("AES decryption initialization failed: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError encrypt(const Byte *in, USize len, Byte *out) const
        {
            int truncated = (int) len;
            if (EVP_EncryptUpdate(this->encryptor, out, &truncated, in, truncated) != 1)
            {
                _print_error("Failed to encrypt: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError decrypt(const Byte *in, USize len, Byte *out) const
        {
            int truncated = (int) len;
            if (EVP_DecryptUpdate(this->decryptor, out, &truncated, in, truncated) != 1)
            {
                _print_error("Failed to decrypt: ");
                return FAIL;
            }

            return SUCCESS;
        }

        ~AES()
        {
            if (this->encryptor != nullptr)
                EVP_CIPHER_CTX_free(this->encryptor);

            if (this->decryptor != nullptr)
                EVP_CIPHER_CTX_free(this->decryptor);
        }
    };

    class RSA
    {
    private:
        enum
        {
            MODULUS_BITS = 1024
        };

        EVP_PKEY_CTX *cipher = nullptr;
        Byte *encodedPublicKey = nullptr;
        USize keySize = 0;

        using KeyPair = EVP_PKEY;

        static IfError generateKeyPair(KeyPair *&keypair)
        {
            EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
            if (ctx == nullptr)
                goto ERR;

            if (EVP_PKEY_keygen_init(ctx) != 1)
                goto ERR;

            if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, MODULUS_BITS) != 1)
                goto ERR;

            EVP_PKEY *pkey;
            pkey = nullptr;
            if (EVP_PKEY_keygen(ctx, &pkey) != 1)
                goto ERR;

            keypair = pkey;

            EVP_PKEY_CTX_free(ctx);
            return SUCCESS;

            ERR:
            keypair = nullptr;

            _print_error("RSA#generateKeyPair() failed: ");

            if (ctx != nullptr)
                EVP_PKEY_CTX_free(ctx);

            return FAIL;
        }

        static IfError encodePublicKey(KeyPair *key, Byte *out, USize &written)
        {
            Byte *str = nullptr;
            I32 len = i2d_PUBKEY(key, &str);
            if (len <= 0)
            {
                _print_error("RSA#encodePublicKey() failed: ");
                return FAIL;
            }

            if (out != nullptr)
                memcpy(out, str, len);

            written = std::max(0, len);

            OPENSSL_free(str);

            return SUCCESS;
        }

    public:
        RSA() = default;

        IfError init(bool encrypting)
        {
            KeyPair *keypair;
            if (generateKeyPair(keypair))
                return FAIL;

            if (encodePublicKey(keypair, nullptr, this->keySize))
                return FAIL;
            this->encodedPublicKey = new Byte[this->keySize];
            if (encodePublicKey(keypair, this->encodedPublicKey, this->keySize))
                return FAIL;

            this->cipher = EVP_PKEY_CTX_new(keypair, nullptr);

            if (this->cipher == nullptr)
            {
                _print_error("Could not create cipher context for RSA: ");
                return FAIL;
            }

            if (encrypting)
            {
                if (EVP_PKEY_encrypt_init(this->cipher) != 1)
                {
                    _print_error("RSA encryption initialization failed: ");
                    return FAIL;
                }
            } else
            {
                if (EVP_PKEY_decrypt_init(this->cipher) != 1)
                {
                    _print_error("RSA decryption initialization failed: ");
                    return FAIL;
                }
            }

            if (EVP_PKEY_CTX_set_rsa_padding(this->cipher, RSA_PKCS1_PADDING) != 1)
            {
                _print_error("Unable to set RSA padding: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError encrypt(const Byte *in, USize len, Byte *out, USize &written) const
        {
            if (EVP_PKEY_encrypt(this->cipher, out, &written, in, len) != 1)
            {
                _print_error("Failed to encrypt: ");
                return FAIL;
            }

            return SUCCESS;
        }

        IfError decrypt(const Byte *in, USize len, Byte *out, USize &written) const
        {
            if (EVP_PKEY_decrypt(this->cipher, out, &written, in, len) != 1)
            {
                _print_error("Failed to decrypt: ");
                return FAIL;
            }

            return SUCCESS;
        }

        [[nodiscard]] Byte *getEncodedPublicKey() const
        {
            return this->encodedPublicKey;
        }

        [[nodiscard]] USize getEncodedKeySize() const
        {
            return this->keySize;
        }

        ~RSA()
        {
            if (this->cipher != nullptr)
                EVP_PKEY_CTX_free(this->cipher);

            delete[] this->encodedPublicKey;
        }
    };

    namespace Base64
    {
        static inline IfError encode(const Byte *input, USize len, std::string &out)
        {
            auto out_len = 4 * ((len + 2) / 3);
            auto bytes = new Byte[out_len + 1]{};
            if (EVP_EncodeBlock(bytes, input, (int) len) != out_len)
            {
                _print_error("Base64#encode() failed: ");
                delete[] bytes;
                out = "";

                return FAIL;
            }

            out = std::string((char *) bytes);
            delete[] bytes;

            return SUCCESS;
        }

        // out[] must be able to hold at least 3 * len / 4 bytes
        static inline IfError decode(const std::string &input, USize len, Byte *out)
        {
            if (EVP_DecodeBlock(out, reinterpret_cast<const Byte *>(input.data()), (int) len) != 3 * len / 4)
            {
                _print_error("Base64#decode() failed: ");
                return FAIL;
            }

            return SUCCESS;
        }
    }
}

#endif // COMIC_CRYPTO_HPP
