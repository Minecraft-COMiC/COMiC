#ifndef COMIC_COMPRESSION_HPP
#define COMIC_COMPRESSION_HPP

#include <zlib.h>
#include <COMiC/core.hpp>

namespace COMiC::Compression
{
    enum
    {
        CHUNK_SIZE = 16384,
    };

    // TODO: handle errors
    class Deflater
    {
    private:
        z_stream stream{};
        char *in = nullptr, *out = nullptr;

    public:
        Deflater() = default;

        IfError init()
        {
            this->stream.zalloc = Z_NULL;
            this->stream.zfree = Z_NULL;
            this->stream.opaque = Z_NULL;

            if (deflateInit2(&this->stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS, 8, Z_DEFAULT_STRATEGY) !=
                Z_OK)
            {
                std::cerr << "Deflate initialization failed: " << this->stream.msg << std::endl;
                return FAIL;
            }

            this->in = new char[CHUNK_SIZE];
            this->out = new char[CHUNK_SIZE];

            return SUCCESS;
        }

        IfError compress(const Byte *input, USize len, std::string &output)
        {
            for (U32 i = 0; i < len; i += CHUNK_SIZE)
            {
                uInt size = (uInt) std::min(len - i, (USize) CHUNK_SIZE);
                memcpy(this->in, input + i, size);
                this->stream.avail_in = size;
                this->stream.next_in = (Bytef *) this->in;

                do
                {
                    this->stream.avail_out = CHUNK_SIZE;
                    this->stream.next_out = (Bytef *) this->out;

                    switch (deflate(&this->stream, Z_FINISH))
                    {
                        case Z_STREAM_END:
                        case Z_OK:
                        case Z_BUF_ERROR:
                            break;
                        default:
                            std::cerr << "Zlib compression failed: " << this->stream.msg << std::endl;
                            return FAIL;
                    }

                    output += std::string(this->out, CHUNK_SIZE - this->stream.avail_out);
                }
                while (this->stream.avail_out == 0);
            }

            if (deflateReset(&this->stream) != Z_OK)
            {
                std::cerr << "Failed to reset Deflater: " << this->stream.msg << std::endl;
                return FAIL;
            }

            return SUCCESS;
        }

        ~Deflater()
        {
            if (deflateEnd(&this->stream) != Z_OK)
                std::cerr << "An error occurred on while finalising Deflater: " << this->stream.msg << std::endl;

            delete[] this->in;
            delete[] this->out;
        }
    };

    class Inflater
    {
    private:
        z_stream stream{};
        char *in = nullptr, *out = nullptr;

    public:
        Inflater() = default;

        IfError init()
        {
            this->stream.zalloc = Z_NULL;
            this->stream.zfree = Z_NULL;
            this->stream.opaque = Z_NULL;
            this->stream.avail_in = 0;
            this->stream.next_in = Z_NULL;

            if (inflateInit2(&this->stream, MAX_WBITS) != Z_OK)
            {
                std::cerr << "Inflater initialization failed: " << this->stream.msg << std::endl;
                return FAIL;
            }

            this->in = new char[CHUNK_SIZE];
            this->out = new char[CHUNK_SIZE];

            return SUCCESS;
        }

        IfError decompress(const Byte *input, USize len, std::string &output)
        {
            for (U32 i = 0; i < len; i += CHUNK_SIZE)
            {
                uInt size = (uInt) std::min(len - i, (USize) CHUNK_SIZE);
                memcpy(this->in, input + i, size);
                this->stream.avail_in = size;
                this->stream.next_in = (Bytef *) this->in;
                if (this->stream.avail_in == 0)
                    break;

                do
                {
                    this->stream.avail_out = CHUNK_SIZE;
                    this->stream.next_out = (Bytef *) this->out;

                    switch (inflate(&this->stream, Z_PARTIAL_FLUSH))
                    {
                        case Z_STREAM_END:
                        case Z_OK:
                        case Z_NEED_DICT:
                            break;
                        default:
                            std::cerr << "Zlib decompression failed: " << this->stream.msg << std::endl;
                            return FAIL;
                    }

                    output += std::string(this->out, CHUNK_SIZE - this->stream.avail_out);
                }
                while (this->stream.avail_out == 0);
            }

            if (inflateReset(&this->stream) != Z_OK)
            {
                std::cerr << "Failed to reset Inflater: " << this->stream.msg << std::endl;
                return FAIL;
            }

            return SUCCESS;
        }

        ~Inflater()
        {
            if (inflateEnd(&this->stream) != Z_OK)
                std::cerr << "An error occurred while finalising Inflater: " << this->stream.msg << std::endl;

            delete[] in;
            delete[] out;
        }
    };

    inline Inflater INFLATER;
    inline Deflater DEFLATER;

    static inline IfError init()
    {
        if (INFLATER.init() || DEFLATER.init())
        {
            std::cerr << "Failed to initialize compression" << std::endl;
            return FAIL;
        }

        std::cout << "Using `Zlib " ZLIB_VERSION "` as COMiC::Compression API" << std::endl;

        return SUCCESS;
    }
}

#endif //COMIC_COMPRESSION_HPP
