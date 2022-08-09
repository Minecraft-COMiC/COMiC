#ifndef COMIC_IO_HPP
#define COMIC_IO_HPP

#include <string>
#include "core/types.hpp"

namespace COMiC::IO
{
    class IOError : public Error
    {
    protected:
        std::string message;

    public:
        explicit IOError(const std::string &str) : IOError(str.c_str())
        {
        }

        explicit IOError(const char *msg)
        {
            this->message = "COMiC::IO: ";
            this->message.append(msg);
        }

        [[nodiscard]] const char *what() const noexcept override
        {
            return message.c_str();
        }
    };

    std::string readFile(const std::string &filename, bool binary = false);

    void writeFile(const std::string &filename, const std::string &data, bool binary = false);
}

#endif //COMIC_IO_HPP
