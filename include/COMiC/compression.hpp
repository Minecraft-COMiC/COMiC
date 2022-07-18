#ifndef COMIC_COMPRESSION_HPP
#define COMIC_COMPRESSION_HPP

#include <zlib.h>
#include <COMiC/core.hpp>

namespace COMiC::Compression
{
    enum
    {
        CHUNK_SIZE = 16384,
        COMPRESSION_THRESHOLD = 256
    };

    class Deflater
    {
    private:
        enum
        {
            MAXIMUM_COMPRESSION_LEVEL = 9
        };

        z_stream stream{};
        char *in, *out;

        void start(const Byte *input, USize len, std::string &result)
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

                    switch (deflate(&this->stream, Z_NO_FLUSH))
                    {

                    }

                    result += std::string(this->out, CHUNK_SIZE - this->stream.avail_out);
                }
                while (this->stream.avail_out == 0);
            }
        }

        void finish(std::string &result)
        {
            this->stream.avail_in = 0;
            this->stream.next_in = (Bytef *) this->in;

            do
            {
                this->stream.avail_out = CHUNK_SIZE;
                this->stream.next_out = (Bytef *) this->out;

                switch (deflate(&this->stream, Z_FINISH))
                {

                }

                result += std::string(this->out, CHUNK_SIZE - this->stream.avail_out);
            }
            while (this->stream.avail_out == 0);
        }

    public:
        Deflater()
        {
            this->stream.zalloc = Z_NULL;
            this->stream.zfree = Z_NULL;
            this->stream.opaque = Z_NULL;

            switch (deflateInit(&this->stream, MAXIMUM_COMPRESSION_LEVEL))
            {

            }

            in = new char[CHUNK_SIZE];
            out = new char[CHUNK_SIZE];
        }

        void compress(const Byte *input, USize len, std::string &output)
        {
            start(input, len, output);
            finish(output);

            deflateReset(&this->stream);
        }

        ~Deflater()
        {
            deflateEnd(&this->stream);
        }
    };

    class Inflater
    {
    private:
        z_stream stream{};
        char *in, *out;

    public:
        Inflater()
        {
            this->stream.zalloc = Z_NULL;
            this->stream.zfree = Z_NULL;
            this->stream.opaque = Z_NULL;
            this->stream.avail_in = 0;
            this->stream.next_in = Z_NULL;

            switch (inflateInit(&this->stream))
            {

            }

            in = new char[CHUNK_SIZE];
            out = new char[CHUNK_SIZE];
        }

        void decompress(const Byte *input, USize len, std::string &result)
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

                    switch (inflate(&this->stream, Z_NO_FLUSH))
                    {

                    }

                    result += std::string(this->out, CHUNK_SIZE - this->stream.avail_out);
                }
                while (this->stream.avail_out == 0);
            }

            inflateReset(&this->stream);
        }

        ~Inflater()
        {
            inflateEnd(&this->stream);
            delete[] in;
            delete[] out;
        }
    };
}

#endif //COMIC_COMPRESSION_HPP
