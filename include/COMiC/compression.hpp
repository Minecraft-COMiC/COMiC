#ifndef COMIC_COMPRESSION_HPP
#define COMIC_COMPRESSION_HPP

#include <COMiC/core.hpp>

namespace COMiC::Compression
{
    static constexpr USize WINDOW_SIZE = 1ULL << 15;

    class CompressionError : public Error
    {
    protected:
        std::string message;

    public:
        CompressionError(const char *msg, const char *zlib)
        {
            this->message = "COMiC::Compression: ";
            this->message.append(msg);
            if (zlib != nullptr)
            {
                this->message.append(" (");
                this->message.append(zlib);
                this->message.append(")");
            }
            else
                this->message.append(" (Unknown Zlib error)");
        }

        [[nodiscard]] const char *what() const noexcept override
        {
            return message.c_str();
        }
    };

    class Deflater
    {
    private:
        std::any stream;
        Byte in[WINDOW_SIZE]{}, out[WINDOW_SIZE]{};

    public:
        Deflater();

        ByteVector compress(const Byte *input, USize len);

        ~Deflater();
    };

    class Inflater
    {
    private:
        std::any stream;
        Byte in[WINDOW_SIZE]{}, out[WINDOW_SIZE]{};

    public:
        Inflater();

        ByteVector decompress(const Byte *input, USize len);

        ~Inflater();
    };

    inline Inflater INFLATER;
    inline Deflater DEFLATER;

    void init();
}

#endif //COMIC_COMPRESSION_HPP
