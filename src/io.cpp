#include <COMiC/core.hpp>
#include <COMiC/io.hpp>

namespace COMiC::IO
{
    // TODO: Error handling
    std::string readFile(const std::string &filename, bool binary)
    {
        std::shared_ptr<FILE> file(
                fopen(filename.c_str(), binary ? "rb" : "r"),
                [](FILE *f) { if (f != nullptr) fclose(f); }
        );

        if (file == nullptr)
            throw IOError("Could not open file `" + filename + "` for reading");

        if (fseek(file.get(), 0, SEEK_END) == 0)
        {
            auto size = ftell(file.get());
            if (size == -1)
                throw IOError("ftell()");

            if (fseek(file.get(), 0, SEEK_SET) != 0)
                throw IOError("fseek()");

            std::string out;
            out.resize(size);
            if (fread(out.data(), 1, size, file.get()) <= 0)
                throw IOError("fread()");

            return out;
        }

        throw IOError("Failed to read file `" + filename + '`');
    }

    void writeFile(const std::string &filename, const std::string &data, bool binary)
    {
        std::shared_ptr<FILE> file(fopen(filename.c_str(), binary ? "wb" : "w"), fclose);
        if (file == nullptr)
            throw IOError("Could not open file `" + filename + "` for writing");

        if (fwrite(data.data(), 1, data.size(), file.get()) != data.size())
            throw IOError("Failed to write to a `" + filename + "` file");
    }
}
