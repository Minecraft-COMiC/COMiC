#include "buffer.h"

COMiC_Constructor
COMiC_Network_ByteBuffer COMiC_Network_Buffer_Init(
        COMiC_In unsigned int capacity
)
{
    return (COMiC_Network_ByteBuffer) {malloc(capacity), 5, capacity};
}

COMiC_Constructor
COMiC_Network_ByteBuffer COMiC_Network_Buffer_InitWithDefaultCapacity(void)
{
    return COMiC_Network_Buffer_Init(100);
}

unsigned int GetVarIntSizeInBytes(int i)
{
    for (int j = 1; j < 5; j++)
    {
        if ((i & -1 << j * 7) != 0) continue;
        return j;
    }

    return 5;
}

void COMiC_Network_Buffer_Prepare(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    self->index -= 5;
    unsigned int data_size = self->index;

    unsigned int offset = GetVarIntSizeInBytes((int) self->index);
    self->index = 5 - offset;
    COMiC_Network_Buffer_WriteVarInt(self, (int) data_size);
    self->bytes += (5 - offset);

    self->size = data_size + offset;
}

void COMiC_Network_Buffer_SkipBytes(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In unsigned int count
)
{
    self->index = self->index + count;
}

char COMiC_Network_Buffer_ReadByte(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return self->bytes[self->index++];
}

void COMiC_Network_Buffer_WriteByte(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In char byte
)
{
    if (self->index + 1 > self->size)
    {
        self->bytes = realloc(self->bytes, self->size * 2);
        self->size *= 2;
    }

    self->bytes[self->index++] = byte;
}

int COMiC_Network_Buffer_ReadVarInt(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    int res = 0;
    char byte;

    for (int i = 0; i < 6; i++)
    {
        byte = COMiC_Network_Buffer_ReadByte(self);
        res |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return res;
}

void COMiC_Network_Buffer_WriteVarInt(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
)
{
    for (int i = 0; i < 6; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            COMiC_Network_Buffer_WriteByte(self, (char) value);
            return;
        }

        COMiC_Network_Buffer_WriteByte(self, (char) ((value & 0x7F) | 0x80));
        value = (int) ((unsigned int) value >> 7);
    }
}

long long COMiC_Network_Buffer_ReadVarLong(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    long long value = 0;
    char byte;

    for (int i = 0; i < 11; i++)
    {
        byte = COMiC_Network_Buffer_ReadByte(self);
        value |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return value;
}

void COMiC_Network_Buffer_WriteVarLong(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In long long value
)
{
    for (int i = 0; i < 11; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            COMiC_Network_Buffer_WriteByte(self, (char) value);
            return;
        }

        COMiC_Network_Buffer_WriteByte(self, (char) ((value & 0x7F) | 0x80));
        value = (long long) ((unsigned long long) value >> 7);
    }
}

COMiC_Bool COMiC_Network_Buffer_ReadBool(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return COMiC_Network_Buffer_ReadByte(self) != 0;
}

void COMiC_Network_Buffer_WriteBool(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
)
{
    COMiC_Network_Buffer_WriteByte(self, (char) (value != 0));
}

short COMiC_Network_Buffer_ReadShort(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return (short) ((COMiC_Network_Buffer_ReadByte(self) & 0xFF) << 8 | COMiC_Network_Buffer_ReadByte(self) & 0xFF);
}

void COMiC_Network_Buffer_WriteShort(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In short value
)
{
    COMiC_Network_Buffer_WriteByte(self, (char) ((unsigned short) value >> 8));
    COMiC_Network_Buffer_WriteByte(self, (char) value);
}

int COMiC_Network_Buffer_ReadInt(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return (COMiC_Network_Buffer_ReadShort(self) & 0xFFFF) << 16 | COMiC_Network_Buffer_ReadShort(self) & 0xFFFF;
}

void COMiC_Network_Buffer_WriteInt(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
)
{
    COMiC_Network_Buffer_WriteShort(self, (short) ((unsigned int) value >> 16));
    COMiC_Network_Buffer_WriteShort(self, (short) value);
}

long long COMiC_Network_Buffer_ReadLong(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return (long long) (COMiC_Network_Buffer_ReadInt(self) & 0xFFFFFFFFL) << 32 |
           COMiC_Network_Buffer_ReadInt(self) & 0xFFFFFFFFL;
}

void COMiC_Network_Buffer_WriteLong(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In long long value
)
{
    COMiC_Network_Buffer_WriteInt(self, (int) ((unsigned long long) value >> 32));
    COMiC_Network_Buffer_WriteInt(self, (int) value);
}

float COMiC_Network_Buffer_ReadFloat(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    int v = COMiC_Network_Buffer_ReadInt(self);

    union
    {
        int i;
        float f;
    } u;
    u.i = v;

    return u.f;
}

void COMiC_Network_Buffer_WriteFloat(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In float value
)
{
    union
    {
        int i;
        float f;
    } u;
    u.f = value;

    COMiC_Network_Buffer_WriteInt(self, u.i);
}

double COMiC_Network_Buffer_ReadDouble(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    long long v = COMiC_Network_Buffer_ReadLong(self);

    union
    {
        long long l;
        double d;
    } u;
    u.l = v;

    return u.d;
}

void COMiC_Network_Buffer_WriteDouble(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In double value
)
{
    union
    {
        long long l;
        double d;
    } u;
    u.d = value;

    COMiC_Network_Buffer_WriteLong(self, u.l);
}

void COMiC_Network_Buffer_ReadString(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_Out char *out,
        COMiC_In unsigned int max_length
)
{
    int str_len = COMiC_Network_Buffer_ReadVarInt(self);

    if (str_len > max_length * 4)
        return;

    memcpy(out, self->bytes + self->index, str_len);
    out[str_len] = 0;

    COMiC_Network_Buffer_SkipBytes(self, str_len);
}

void COMiC_Network_Buffer_WriteString(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In char *str,
        COMiC_In unsigned int max_length
)
{
    unsigned int str_len = strlen(str);

    if (str_len > max_length) return;

    COMiC_Network_Buffer_WriteVarInt(self, (int) str_len);

    for (int j = 0; j < str_len; j++)
        COMiC_Network_Buffer_WriteByte(self, str[j]);
}

int COMiC_Network_Buffer_ReadEnum(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return COMiC_Network_Buffer_ReadVarInt(self);
}

void COMiC_Network_Buffer_WriteEnum(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
)
{
    COMiC_Network_Buffer_WriteVarInt(self, value);
}

void COMiC_Network_Buffer_ReadByteArray(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_Out char *out,
        COMiC_In unsigned int count
)
{
    memcpy(out, self->bytes + self->index, count);

    COMiC_Network_Buffer_SkipBytes(self, count);
}

void COMiC_Network_Buffer_WriteByteArray(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In char *arr,
        COMiC_In unsigned int count
)
{
    COMiC_Network_Buffer_WriteVarInt(self, (int) count);

    for (int i = 0; i < count; i++)
        COMiC_Network_Buffer_WriteByte(self, arr[i]);
}

void COMiC_Network_Buffer_WriteRSAKey(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In EVP_PKEY *key
)
{
    BIO *publicBIO = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(publicBIO, key);
    int publicKeyLen = BIO_pending(publicBIO);
    char *publicKeyStr = (char *) malloc(publicKeyLen);
    BIO_read(publicBIO, publicKeyStr, publicKeyLen);

    COMiC_Network_Buffer_WriteByteArray(self, publicKeyStr, publicKeyLen);
    OPENSSL_free(publicBIO);
    free(publicKeyStr);
}

int COMiC_Network_Buffer_ReadPacketID(
        COMiC_Out COMiC_Network_ByteBuffer *self
)
{
    return COMiC_Network_Buffer_ReadVarInt(self);
}

void COMiC_Network_Buffer_WritePacketID(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int id
)
{
    COMiC_Network_Buffer_WriteVarInt(self, (int) id & 0xFF);
}
