#include <cstring>
#include <cstdlib>
#include <COMiC/core.hpp>
#include "COMiC/network.hpp"

namespace COMiC::Network
{
    U32 GetVarIntSizeInBytes(I32 i)
    {
        for (I32 j = 1; j < 5; j++)
        {
            if ((i << j * 7) != 0)
                continue;

            return j;
        }

        return 5;
    }

    void Buffer::prepare()
    {
        this->index -= 5;
        size_t data_size = this->index;

        U32 offset = GetVarIntSizeInBytes((I32) this->index);

        this->index = 5 - offset;
        writeVarInt((I32) data_size);
        this->bytes += (5 - offset);

        this->size = data_size + offset;
    }

    void Buffer::skip(size_t count)
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
        return (I64) (((readInt() & 0xFFFFFFFFULL) << 32) | (readInt() & 0xFFFFFFFFUL));
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

    void Buffer::readString(char *out, size_t maxlen)
    {
        I32 strLen = readVarInt();

        if (strLen > maxlen * 4)
            return;

        memcpy(out, this->bytes + this->index, strLen);
        out[strLen] = 0;

        skip(strLen);
    }

    void Buffer::writeString(const char *str, size_t maxlen)
    {
        U32 strLen = strlen(str);

        if (strLen > maxlen) return;

        writeVarInt((I32) strLen);

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
