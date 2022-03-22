#include <string.h>
#include <stdlib.h>
#include <COMiC.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

COMiC_Constructor
COMiC_Network_Buffer COMiC_Network_Buffer_Init(
        COMiC_In COMiC_UInt32 capacity
)
{
    return (COMiC_Network_Buffer) {malloc(capacity), 5, capacity};
}

COMiC_Constructor
COMiC_Network_Buffer COMiC_Network_Buffer_InitWithDefaultCapacity(void)
{
    return COMiC_Network_Buffer_Init(100);
}

COMiC_UInt32 GetVarIntSizeInBytes(COMiC_Int32 i)
{
    for (COMiC_Int32 j = 1; j < 5; j++)
    {
        if ((i & -1 << j * 7) != 0) continue;
        return j;
    }

    return 5;
}

void COMiC_Network_Buffer_Prepare(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    self->index -= 5;
    COMiC_UInt32 data_size = self->index;

    COMiC_UInt32 offset = GetVarIntSizeInBytes((COMiC_Int32) self->index);
    self->index = 5 - offset;
    COMiC_Network_Buffer_WriteVarInt(self, (COMiC_Int32) data_size);
    self->bytes += (5 - offset);

    self->size = data_size + offset;
}

void COMiC_Network_Buffer_SkipBytes(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_UInt32 count
)
{
    self->index = self->index + count;
}

COMiC_Int8 COMiC_Network_Buffer_ReadByte(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return self->bytes[self->index++];
}

void COMiC_Network_Buffer_WriteByte(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int8 byte
)
{
    if (self->index + 1 > self->size)
    {
        self->bytes = realloc(self->bytes, self->size * 2);
        self->size *= 2;
    }

    self->bytes[self->index++] = byte;
}

COMiC_Int32 COMiC_Network_Buffer_ReadVarInt(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    COMiC_Int32 res = 0;
    COMiC_Int8 byte;

    for (COMiC_Int32 i = 0; i < 6; i++)
    {
        byte = COMiC_Network_Buffer_ReadByte(self);
        res |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return res;
}

void COMiC_Network_Buffer_WriteVarInt(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
)
{
    for (COMiC_Int32 i = 0; i < 6; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) value);
            return;
        }

        COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) ((value & 0x7F) | 0x80));
        value = (COMiC_Int32) ((COMiC_UInt32) value >> 7);
    }
}

COMiC_Int64 COMiC_Network_Buffer_ReadVarLong(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    COMiC_Int64 value = 0;
    COMiC_Int8 byte;

    for (COMiC_Int32 i = 0; i < 11; i++)
    {
        byte = COMiC_Network_Buffer_ReadByte(self);
        value |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return value;
}

void COMiC_Network_Buffer_WriteVarLong(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int64 value
)
{
    for (COMiC_Int32 i = 0; i < 11; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) value);
            return;
        }

        COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) ((value & 0x7F) | 0x80));
        value = (COMiC_Int64) ((COMiC_UInt64) value >> 7);
    }
}

COMiC_Bool COMiC_Network_Buffer_ReadBool(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return COMiC_Network_Buffer_ReadByte(self) != 0;
}

void COMiC_Network_Buffer_WriteBool(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
)
{
    COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) (value != 0));
}

COMiC_Int16 COMiC_Network_Buffer_ReadShort(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return (COMiC_Int16) ((COMiC_Network_Buffer_ReadByte(self) & 0xFF) << 8 | COMiC_Network_Buffer_ReadByte(self) & 0xFF);
}

void COMiC_Network_Buffer_WriteShort(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int16 value
)
{
    COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) ((COMiC_UInt16) value >> 8));
    COMiC_Network_Buffer_WriteByte(self, (COMiC_Int8) value);
}

COMiC_Int32 COMiC_Network_Buffer_ReadInt(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return (COMiC_Network_Buffer_ReadShort(self) & 0xFFFF) << 16 | COMiC_Network_Buffer_ReadShort(self) & 0xFFFF;
}

void COMiC_Network_Buffer_WriteInt(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
)
{
    COMiC_Network_Buffer_WriteShort(self, (COMiC_Int16) ((COMiC_UInt32) value >> 16));
    COMiC_Network_Buffer_WriteShort(self, (COMiC_Int16) value);
}

COMiC_Int64 COMiC_Network_Buffer_ReadLong(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return (COMiC_Int64) (COMiC_Network_Buffer_ReadInt(self) & 0xFFFFFFFFL) << 32 |
           COMiC_Network_Buffer_ReadInt(self) & 0xFFFFFFFFL;
}

void COMiC_Network_Buffer_WriteLong(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int64 value
)
{
    COMiC_Network_Buffer_WriteInt(self, (COMiC_Int32) ((COMiC_UInt64) value >> 32));
    COMiC_Network_Buffer_WriteInt(self, (COMiC_Int32) value);
}

COMiC_Float COMiC_Network_Buffer_ReadFloat(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    COMiC_Int32 v = COMiC_Network_Buffer_ReadInt(self);

    union
    {
        COMiC_Int32 i;
        COMiC_Float f;
    } u;
    u.i = v;

    return u.f;
}

void COMiC_Network_Buffer_WriteFloat(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Float value
)
{
    union
    {
        COMiC_Int32 i;
        COMiC_Float f;
    } u;
    u.f = value;

    COMiC_Network_Buffer_WriteInt(self, u.i);
}

COMiC_Double COMiC_Network_Buffer_ReadDouble(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    COMiC_Int64 v = COMiC_Network_Buffer_ReadLong(self);

    union
    {
        COMiC_Int64 l;
        COMiC_Double d;
    } u;
    u.l = v;

    return u.d;
}

void COMiC_Network_Buffer_WriteDouble(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Double value
)
{
    union
    {
        COMiC_Int64 l;
        COMiC_Double d;
    } u;
    u.d = value;

    COMiC_Network_Buffer_WriteLong(self, u.l);
}

void COMiC_Network_Buffer_ReadString(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_Out COMiC_String out,
        COMiC_In COMiC_UInt32 max_length
)
{
    COMiC_Int32 str_len = COMiC_Network_Buffer_ReadVarInt(self);

    if (str_len > max_length * 4)
        return;

    memcpy(out, self->bytes + self->index, str_len);
    out[str_len] = 0;

    COMiC_Network_Buffer_SkipBytes(self, str_len);
}

void COMiC_Network_Buffer_WriteString(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_String str,
        COMiC_In COMiC_UInt32 max_length
)
{
    COMiC_UInt32 str_len = strlen(str);

    if (str_len > max_length) return;

    COMiC_Network_Buffer_WriteVarInt(self, (COMiC_Int32) str_len);

    for (COMiC_Int32 j = 0; j < str_len; j++)
        COMiC_Network_Buffer_WriteByte(self, str[j]);
}

COMiC_Int32 COMiC_Network_Buffer_ReadEnum(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return COMiC_Network_Buffer_ReadVarInt(self);
}

void COMiC_Network_Buffer_WriteEnum(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
)
{
    COMiC_Network_Buffer_WriteVarInt(self, value);
}

void COMiC_Network_Buffer_ReadByteArray(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_Out COMiC_Int8 *out,
        COMiC_In COMiC_UInt32 count
)
{
    memcpy(out, self->bytes + self->index, count);

    COMiC_Network_Buffer_SkipBytes(self, count);
}

void COMiC_Network_Buffer_WriteByteArray(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int8 *arr,
        COMiC_In COMiC_UInt32 count
)
{
    COMiC_Network_Buffer_WriteVarInt(self, (COMiC_Int32) count);

    for (COMiC_Int32 i = 0; i < count; i++)
        COMiC_Network_Buffer_WriteByte(self, arr[i]);
}

void COMiC_Network_Buffer_WriteRSAKey(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In EVP_PKEY *key
)
{
    BIO *publicBIO = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(publicBIO, key);
    COMiC_Int32 publicKeyLen = BIO_pending(publicBIO);
    COMiC_String publicKeyStr = malloc(publicKeyLen);
    BIO_read(publicBIO, publicKeyStr, publicKeyLen);

    COMiC_Network_Buffer_WriteByteArray(self, (COMiC_Int8 *) publicKeyStr, publicKeyLen);
    OPENSSL_free(publicBIO);
    free(publicKeyStr);
}

COMiC_Int32 COMiC_Network_Buffer_ReadPacketID(
        COMiC_Out COMiC_Network_Buffer *self
)
{
    return COMiC_Network_Buffer_ReadVarInt(self);
}

void COMiC_Network_Buffer_WritePacketID(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 id
)
{
    COMiC_Network_Buffer_WriteVarInt(self, (COMiC_Int32) id & 0xFF);
}
