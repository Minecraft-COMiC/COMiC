#include <zlib.h>
#include <COMiC/compression.hpp>

#define STREAM std::any_cast<z_stream&>(this->stream) // this->stream
#define STREAM_PTR std::any_cast<z_stream>(&this->stream) // &this->stream

namespace COMiC::Compression
{
    Deflater::Deflater() : stream(std::make_any<z_stream>(z_stream{}))
    {
        STREAM.zalloc = Z_NULL;
        STREAM.zfree = Z_NULL;
        STREAM.opaque = Z_NULL;

        if (deflateInit2(
                STREAM_PTR,
                Z_DEFAULT_COMPRESSION,
                Z_DEFLATED,
                MAX_WBITS,
                8,
                Z_DEFAULT_STRATEGY
            ) != Z_OK)
            throw CompressionError("Deflater initialization failed", STREAM.msg);
    }

    ByteVector Deflater::compress(const Byte *input, USize len)
    {
        ByteVector output;
        for (USize i = 0; i < len; i += WINDOW_SIZE)
        {
            uInt size = (uInt) std::min(len - i, WINDOW_SIZE);
            memcpy(this->in, input + i, size);
            STREAM.avail_in = size;
            STREAM.next_in = (Bytef *) this->in;

            do
            {
                STREAM.avail_out = WINDOW_SIZE;
                STREAM.next_out = (Bytef *) this->out;

                switch (deflate(STREAM_PTR, Z_FINISH))
                {
                    case Z_STREAM_END:
                    case Z_OK:
                    case Z_BUF_ERROR:
                        break;
                    default:
                        throw CompressionError("Zlib compression failed", STREAM.msg);
                }

                output.insert(output.end(), this->out, this->out + (WINDOW_SIZE - STREAM.avail_out));
            }
            while (STREAM.avail_out == 0);
        }

        if (deflateReset(STREAM_PTR) != Z_OK)
            throw CompressionError("Failed to reset Deflater", STREAM.msg);

        return output;
    }

    Deflater::~Deflater()
    {
        if (deflateEnd(STREAM_PTR) != Z_OK)
        {
            if (STREAM.msg == nullptr)
                std::cerr << "An unknown error occurred while finalizing Deflater" << std::endl;
            else
                std::cerr << "An error occurred while finalizing Deflater: " << STREAM.msg << std::endl;
        }
    }

    Inflater::Inflater() : stream(std::make_any<z_stream>(z_stream{}))
    {
        STREAM.zalloc = Z_NULL;
        STREAM.zfree = Z_NULL;
        STREAM.opaque = Z_NULL;
        STREAM.avail_in = 0;
        STREAM.next_in = Z_NULL;

        if (inflateInit2(STREAM_PTR, MAX_WBITS) != Z_OK)
            throw CompressionError("Inflater initialization failed", STREAM.msg);
    }

    ByteVector Inflater::decompress(const Byte *input, USize len)
    {
        ByteVector output;
        for (USize i = 0; i < len; i += WINDOW_SIZE)
        {
            uInt size = (uInt) std::min(len - i, WINDOW_SIZE);
            memcpy(this->in, input + i, size);
            STREAM.avail_in = size;
            STREAM.next_in = (Bytef *) this->in;
            if (STREAM.avail_in == 0)
                break;

            do
            {
                STREAM.avail_out = WINDOW_SIZE;
                STREAM.next_out = (Bytef *) this->out;

                switch (inflate(STREAM_PTR, Z_PARTIAL_FLUSH))
                {
                    case Z_STREAM_END:
                    case Z_OK:
                        break;
                    case Z_NEED_DICT:
                        throw CompressionError(
                            "Inflater requires dictionary for decompression but it was not provided",
                            "Z_NEED_DICT"
                        );
                    default:
                        throw CompressionError("Zlib decompression failed", STREAM.msg);
                }

                output.insert(output.end(), this->out, this->out + (WINDOW_SIZE - STREAM.avail_out));
            }
            while (STREAM.avail_out == 0);
        }

        if (inflateReset(STREAM_PTR) != Z_OK)
            throw CompressionError("Failed to reset Inflater", STREAM.msg);

        return output;
    }

    Inflater::~Inflater()
    {
        if (inflateEnd(STREAM_PTR) != Z_OK)
        {
            if (STREAM.msg == nullptr)
                std::cerr << "An unknown error occurred while finalizing Inflater" << std::endl;
            else
                std::cerr << "An error occurred while finalizing Inflater: " << STREAM.msg << std::endl;
        }
    }

    void init()
    {
        std::cout << "Using `Zlib " ZLIB_VERSION "` as COMiC::Compression API" << std::endl;
    }
}