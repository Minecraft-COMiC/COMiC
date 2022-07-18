#include <cstring>
#include <cstdlib>
#include <COMiC/core.hpp>
#include "COMiC/network.hpp"

namespace COMiC::Network
{
    USize varIntSize(I32 i)
    {
        for (I32 j = 1; j < 5; j++)
        {
            if ((i & -1 << j * 7) != 0)
                continue;

            return j;
        }

        return 5;
    }

    void Buffer::prependSize()
    {
        USize data_size = this->index - DATA_START, offset = varIntSize((I32) data_size);

        this->index = DATA_START - offset;
        writeVarInt((I32) data_size);
        this->index = DATA_START - offset;
        this->size = data_size + offset;
    }

    void Buffer::prepare(ClientNetInfo &connection)
    {
        if (connection.compressed)
        {
            auto data_size = this->length();
            if (data_size >= Compression::COMPRESSION_THRESHOLD)
            {
                // Compressed packet format (https://www.reddit.com/r/admincraft/comments/2agvxn/how_compression_works):
                // 1. Total packet length (VarInt);
                // 2. Uncompressed packet data length (VarInt) - zero if uncompressed;
                // 3. Packet data.

                std::string deflated;
                connection.deflater.compress(this->data(), data_size, deflated);
                this->index = Buffer::DATA_START;
                this->writeVarInt((I32) data_size);
                memcpy(this->data() + this->index, deflated.data(), deflated.length());

                this->skip(deflated.length());
            }
            else
            {
                this->skip(1);
                memmove(this->data() + 1, this->data(), data_size);
                this->bytes[Buffer::DATA_START] = 0;
            }
        }

        this->prependSize();

        if (connection.encrypted)
            connection.cipher.encrypt(this->bytes + this->index, this->size, this->bytes + this->index);
    }

    void Buffer::skip(USize count)
    {
        this->index += count;
    }

    Byte Buffer::read()
    {
        return this->bytes[this->index++];
    }

    void Buffer::write(Byte byte)
    {
        if (this->index + 1 > this->size)
        {
            this->bytes = static_cast<Byte *>(realloc(this->bytes, this->size * 2));
            this->size *= 2;
        }

        this->bytes[this->index++] = byte;
    }

    I32 Buffer::readVarInt()
    {
        I32 res = 0;
        Byte byte;

        for (I32 i = 0; i < 6; i++)
        {
            byte = read();
            res |= (byte & 0x7F) << (i * 7);

            if ((byte & 0x80) != 0x80) break;
        }

        return res;
    }

    void Buffer::writeVarInt(I32 value)
    {
        for (I32 i = 0; i < 6; i++)
        {
            if ((value & ~0x7FL) == 0)
            {
                write((Byte) value);
                return;
            }

            write((Byte) ((value & 0x7F) | 0x80));
            value = (I32) ((U32) value >> 7);
        }
    }

    I64 Buffer::readVarLong()
    {
        I64 value = 0;
        Byte byte;

        for (I32 i = 0; i < 11; i++)
        {
            byte = read();
            value |= (byte & 0x7F) << (i * 7);

            if ((byte & 0x80) != 0x80) break;
        }

        return value;
    }

    void Buffer::writeVarLong(I64 value)
    {
        for (I32 i = 0; i < 11; i++)
        {
            if ((value & ~0x7FL) == 0)
            {
                write((Byte) value);
                return;
            }

            write((Byte) ((value & 0x7F) | 0x80));
            value = (I64) ((U64) value >> 7);
        }
    }

    bool Buffer::readBool()
    {
        return (read() != 0);
    }

    void Buffer::writeBool(bool value)
    {
        write((Byte) value);
    }

    I16 Buffer::readShort()
    {
        return (I16) (((read() & 0xFF) << 8) | (read() & 0xFF));
    }

    void Buffer::writeShort(I16 value)
    {
        write((Byte) ((U16) value >> 8));
        write((Byte) value);
    }

    I32 Buffer::readInt()
    {
        return ((readShort() & 0xFFFF) << 16) | (readShort() & 0xFFFF);
    }

    void Buffer::writeInt(I32 value)
    {
        writeShort((I16) ((U32) value >> 16));
        writeShort((I16) value);
    }

    I64 Buffer::readLong()
    {
        return (I64) (((readInt() & 0xFFFFFFFFULL) << 32) | (readInt() & 0xFFFFFFFFULL));
    }

    void Buffer::writeLong(I64 value)
    {
        writeInt((I32) ((U64) value >> 32));
        writeInt((I32) value);
    }

    float Buffer::readFloat()
    {
        I32 v = readInt();

        union
        {
            I32 i;
            float f;
        } u{v};

        return u.f;
    }

    void Buffer::writeFloat(float value)
    {
        union
        {
            float f;
            I32 i;
        } u{value};

        writeInt(u.i);
    }

    double Buffer::readDouble()
    {
        I64 v = readLong();

        union
        {
            I64 l;
            double d;
        } u = {v};

        return u.d;
    }

    void Buffer::writeDouble(double value)
    {
        union
        {
            double d;
            I64 l;
        } u = {value};

        writeLong(u.l);
    }

    std::string Buffer::readString(USize maxlen)
    {
        I32 strLen = readVarInt();

        if (strLen > maxlen * 4)
            return "";

        auto str = std::string((char *) this->bytes + this->index, strLen);
        skip(strLen);

        return str;
    }

    void Buffer::writeString(const std::string &str, USize maxlen)
    {
        I32 strLen = (I32) str.length();

        if (strLen > maxlen) return;

        writeVarInt(strLen);

        for (I32 j = 0; j < strLen; j++)
            write((Byte) str[j]);
    }

    I32 Buffer::readEnum()
    {
        return readVarInt();
    }

    void Buffer::writeEnum(I32 value)
    {
        writeVarInt(value);
    }

    void Buffer::readByteArray(Byte *out, I32 count)
    {
        memcpy(out, this->bytes + this->index, count);
        skip(count);
    }

    void Buffer::writeByteArray(const Byte *arr, I32 count)
    {
        writeVarInt(count);

        for (I32 i = 0; i < count; i++)
            write(arr[i]);
    }

    PacketID Buffer::readPacketID(NetworkState state)
    {
        return static_cast<PacketID>(((state + 1) << 8) | readVarInt());
    }

    void Buffer::writePacketID(PacketID id)
    {
        writeVarInt((I32) id & 0xFF);
    }
}
