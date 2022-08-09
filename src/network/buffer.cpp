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

    void RecvBuf::skip(USize count)
    {
        this->index += count;
    }

    void SendBuf::prependSize()
    {
        USize data_size = this->index - DATA_START, offset = varIntSize((I32) data_size);

        this->index = DATA_START - offset;
        writeVarInt((I32) data_size);
        this->index = DATA_START - offset;
        this->size = data_size + offset;
    }

    void SendBuf::prepare(ClientNetInfo &connection)
    {
        if (connection.compressed)
        {
            auto data_size = this->length();
            if (data_size >= CONFIG.networkCompressionThreshold())
            {
                // Compressed packet format (https://www.reddit.com/r/admincraft/comments/2agvxn/how_compression_works):
                // 1. Total packet length (VarInt);
                // 2. Uncompressed packet data length (VarInt) - zero if uncompressed;
                // 3. Packet data.

                auto deflated = Compression::DEFLATER.compress(this->data(), data_size);
                this->index = SendBuf::DATA_START;
                this->writeVarInt((I32) data_size);
                memcpy(this->data() + this->index, deflated.data(), deflated.size());

                this->skip(deflated.size());
            }
            else
            {
                this->skip(1);
                memmove(this->data() + 1, this->data(), data_size);
                this->bytes[SendBuf::DATA_START] = 0;
            }
        }

        this->prependSize();

        if (connection.encrypted)
            connection.cipher.encrypt(this->position(), this->size, this->position());
    }

    void SendBuf::skip(USize count)
    {
        this->index += count;
    }

    Byte RecvBuf::read()
    {
        if (this->index >= this->bytes.size())
            throw std::out_of_range("COMiC: No more bytes to read in RecvBuf");

        return this->bytes[this->index++];
    }

    void SendBuf::write(Byte byte)
    {
        if (this->index < DATA_START)
            this->bytes[this->index] = byte;
        else
            this->bytes.push_back(byte);

        this->index++;
    }

    void SendBuf::write(std::initializer_list<Byte> data)
    {
        for (auto byte: data)
            write(byte);
    }

    I32 RecvBuf::readVarInt()
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

    void SendBuf::writeVarInt(I32 value)
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

    I64 RecvBuf::readVarLong()
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

    void SendBuf::writeVarLong(I64 value)
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

    bool RecvBuf::readBool()
    {
        return (read() != 0);
    }

    void SendBuf::writeBool(bool value)
    {
        write((Byte) value);
    }

    I16 RecvBuf::readShort()
    {
        return (I16) (((read() & 0xFF) << 8) | (read() & 0xFF));
    }

    void SendBuf::writeShort(I16 value)
    {
        write((Byte) ((U16) value >> 8));
        write((Byte) value);
    }

    I32 RecvBuf::readInt()
    {
        return ((readShort() & 0xFFFF) << 16) | (readShort() & 0xFFFF);
    }

    void SendBuf::writeInt(I32 value)
    {
        writeShort((I16) ((U32) value >> 16));
        writeShort((I16) value);
    }

    I64 RecvBuf::readLong()
    {
        return (I64) (((readInt() & 0xFFFFFFFFULL) << 32) | (readInt() & 0xFFFFFFFFULL));
    }

    void SendBuf::writeLong(I64 value)
    {
        writeInt((I32) ((U64) value >> 32));
        writeInt((I32) value);
    }

    float RecvBuf::readFloat()
    {
        I32 v = readInt();

        union
        {
            I32 i;
            float f;
        } u{v};

        return u.f;
    }

    void SendBuf::writeFloat(float value)
    {
        union
        {
            float f;
            I32 i;
        } u{value};

        writeInt(u.i);
    }

    double RecvBuf::readDouble()
    {
        I64 v = readLong();

        union
        {
            I64 l;
            double d;
        } u = {v};

        return u.d;
    }

    void SendBuf::writeDouble(double value)
    {
        union
        {
            double d;
            I64 l;
        } u = {value};

        writeLong(u.l);
    }

    std::string RecvBuf::readString(USize maxlen)
    {
        I32 strLen = readVarInt();

        if (strLen > maxlen * 4)
            return "";

        auto str = std::string((char *) this->position(), strLen);
        skip(strLen);

        return str;
    }

    void SendBuf::writeString(const std::string &str, USize maxlen)
    {
        I32 strLen = (I32) str.length();

        if (strLen > maxlen) return;

        writeVarInt(strLen);

        for (I32 j = 0; j < strLen; j++)
            write((Byte) str[j]);
    }

    I32 RecvBuf::readEnum()
    {
        return readVarInt();
    }

    void SendBuf::writeEnum(I32 value)
    {
        writeVarInt(value);
    }

    ByteVector RecvBuf::readByteArray()
    {
        auto size = this->readVarInt();
        ByteVector out(this->position(), this->position() + size);
        skip(size);

        return out;
    }

    void SendBuf::writeByteArray(const ByteVector &arr)
    {
        writeVarInt((I32) arr.size());

        for (Byte b: arr)
            write(b);
    }

    PacketID RecvBuf::readPacketID(NetworkState state)
    {
        return static_cast<PacketID>(((state + 1) << 8) | readVarInt());
    }

    void SendBuf::writePacketID(PacketID id)
    {
        writeVarInt((I32) id & 0xFF);
    }
}