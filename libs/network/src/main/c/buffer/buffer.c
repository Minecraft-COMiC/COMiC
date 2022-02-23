#include "buffer.h"

COMiC_Network_ByteBuffer COMiC_Network_Buffer_new(unsigned int capacity)
{
    return (COMiC_Network_ByteBuffer) {malloc(capacity), 5, capacity};
}

COMiC_Network_ByteBuffer COMiC_Network_Buffer_new_with_default_capacity(void)
{
    return COMiC_Network_Buffer_new(100);
}

unsigned int get_var_int_size_in_bytes(int i)
{
    for (int j = 1; j < 5; j++)
    {
        if ((i & -1 << j * 7) != 0) continue;
        return j;
    }

    return 5;
}

void COMiC_Network_Buffer_prepare(COMiC_Network_ByteBuffer *buf)
{
    buf->index -= 5;
    unsigned int data_size = buf->index;

    unsigned int offset = get_var_int_size_in_bytes((int) buf->index);
    buf->index = 5 - offset;
    COMiC_Network_Buffer_write_var_int(buf, (int) data_size);
    buf->bytes += (5 - offset);

    buf->size = data_size + offset;
}

void COMiC_Network_Buffer_skip_bytes(COMiC_Network_ByteBuffer *buf, unsigned int count)
{
    buf->index = buf->index + count;
}

char COMiC_Network_Buffer_read_byte(COMiC_Network_ByteBuffer *buf)
{
    return buf->bytes[buf->index++];
}

void COMiC_Network_Buffer_write_byte(COMiC_Network_ByteBuffer *buf, char byte)
{
    if (buf->index + 1 > buf->size)
    {
        buf->bytes = realloc(buf->bytes, buf->size * 2);
        buf->size *= 2;
    }

    buf->bytes[buf->index++] = byte;
}

int COMiC_Network_Buffer_read_var_int(COMiC_Network_ByteBuffer *buf)
{
    int res = 0;
    char byte;

    for (int i = 0; i < 6; i++)
    {
        byte = COMiC_Network_Buffer_read_byte(buf);
        res |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return res;
}

void COMiC_Network_Buffer_write_var_int(COMiC_Network_ByteBuffer *buf, int value)
{
    for (int i = 0; i < 6; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            COMiC_Network_Buffer_write_byte(buf, (char) value);
            return;
        }

        COMiC_Network_Buffer_write_byte(buf, (char) ((value & 0x7F) | 0x80));
        value = (int) ((unsigned int) value >> 7);
    }
}

long long COMiC_Network_Buffer_read_var_long(COMiC_Network_ByteBuffer *buf)
{
    long long value = 0;
    char byte;

    for (int i = 0; i < 11; i++)
    {
        byte = COMiC_Network_Buffer_read_byte(buf);
        value |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return value;
}

void COMiC_Network_Buffer_write_var_long(COMiC_Network_ByteBuffer *buf, long long value)
{
    for (int i = 0; i < 11; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            COMiC_Network_Buffer_write_byte(buf, (char) value);
            return;
        }

        COMiC_Network_Buffer_write_byte(buf, (char) ((value & 0x7F) | 0x80));
        value = (long long) ((unsigned long long) value >> 7);
    }
}

int network_buffer_read_bool(COMiC_Network_ByteBuffer *buf)
{
    return COMiC_Network_Buffer_read_byte(buf) != 0;
}

void COMiC_network_buffer_write_bool(COMiC_Network_ByteBuffer *buf, int value)
{
    COMiC_Network_Buffer_write_byte(buf, (char) (value != 0));
}

short COMiC_Network_Buffer_read_short(COMiC_Network_ByteBuffer *buf)
{
    return (short) ((COMiC_Network_Buffer_read_byte(buf) & 0xFF) << 8 | COMiC_Network_Buffer_read_byte(buf) & 0xFF);
}

void COMiC_Network_Buffer_write_short(COMiC_Network_ByteBuffer *buf, short value)
{
    COMiC_Network_Buffer_write_byte(buf, (char) ((unsigned short) value >> 8));
    COMiC_Network_Buffer_write_byte(buf, (char) value);
}

int COMiC_Network_Buffer_read_int(COMiC_Network_ByteBuffer *buf)
{
    return (COMiC_Network_Buffer_read_short(buf) & 0xFFFF) << 16 | COMiC_Network_Buffer_read_short(buf) & 0xFFFF;
}

void COMiC_Network_Buffer_write_int(COMiC_Network_ByteBuffer *buf, int value)
{
    COMiC_Network_Buffer_write_short(buf, (short) ((unsigned int) value >> 16));
    COMiC_Network_Buffer_write_short(buf, (short) value);
}

long long COMiC_Network_Buffer_read_long(COMiC_Network_ByteBuffer *buf)
{
    return (long long) (COMiC_Network_Buffer_read_int(buf) & 0xFFFFFFFFL) << 32 |
            COMiC_Network_Buffer_read_int(buf) & 0xFFFFFFFFL;
}

void COMiC_Network_Buffer_write_long(COMiC_Network_ByteBuffer *buf, long long value)
{
    COMiC_Network_Buffer_write_int(buf, (int) ((unsigned long long) value >> 32));
    COMiC_Network_Buffer_write_int(buf, (int) value);
}

float COMiC_Network_Buffer_read_float(COMiC_Network_ByteBuffer *buf)
{
    int v = COMiC_Network_Buffer_read_int(buf);

    union
    {
        int i;
        float f;
    } u;
    u.i = v;

    return u.f;
}

void COMiC_Network_Buffer_write_float(COMiC_Network_ByteBuffer *buf, float value)
{
    union
    {
        int i;
        float f;
    } u;
    u.f = value;

    COMiC_Network_Buffer_write_int(buf, u.i);
}

double COMiC_Network_Buffer_read_double(COMiC_Network_ByteBuffer *buf)
{
    long long v = COMiC_Network_Buffer_read_long(buf);

    union
    {
        long long l;
        double d;
    } u;
    u.l = v;

    return u.d;
}

void COMiC_Network_Buffer_write_double(COMiC_Network_ByteBuffer *buf, double value)
{
    union
    {
        long long l;
        double d;
    } u;
    u.d = value;

    COMiC_Network_Buffer_write_long(buf, u.l);
}

void COMiC_Network_Buffer_read_string(COMiC_Network_ByteBuffer *buf, char *out, unsigned int max_length)
{
    int str_len = COMiC_Network_Buffer_read_var_int(buf);

    if (str_len > max_length * 4)
        return;

    memcpy(out, buf->bytes + buf->index, str_len);
    out[str_len] = 0;

    COMiC_Network_Buffer_skip_bytes(buf, str_len);
}

void network_buffer_write_string(COMiC_Network_ByteBuffer *buf, char *str, unsigned int max_length)
{
    unsigned int str_len = strlen(str);

    if (str_len > max_length) return;

    COMiC_Network_Buffer_write_var_int(buf, (int) str_len);

    for (int j = 0; j < str_len; j++)
        COMiC_Network_Buffer_write_byte(buf, str[j]);
}

int COMiC_Network_Buffer_read_enum(COMiC_Network_ByteBuffer *buf)
{
    return COMiC_Network_Buffer_read_var_int(buf);
}

void COMiC_Network_Buffer_write_enum(COMiC_Network_ByteBuffer *buf, int value)
{
    COMiC_Network_Buffer_write_var_int(buf, value);
}

void COMiC_Network_Buffer_read_byte_array(COMiC_Network_ByteBuffer *buf, char *out, unsigned int count)
{
    memcpy(out, buf->bytes + buf->index, count);

    COMiC_Network_Buffer_skip_bytes(buf, count);
}

void COMiC_Network_Buffer_write_byte_array(COMiC_Network_ByteBuffer *buf, char *arr, unsigned int count)
{
    COMiC_Network_Buffer_write_var_int(buf, (int) count);

    for (int i = 0; i < count; i++)
        COMiC_Network_Buffer_write_byte(buf, arr[i]);
}

void COMiC_Network_Buffer_write_rsa_key(COMiC_Network_ByteBuffer *buf, EVP_PKEY *key)
{
    BIO *publicBIO = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(publicBIO, key);
    int publicKeyLen = BIO_pending(publicBIO);
    char *publicKeyStr = (char *) malloc(publicKeyLen);
    BIO_read(publicBIO, publicKeyStr, publicKeyLen);

    COMiC_Network_Buffer_write_byte_array(buf, publicKeyStr, publicKeyLen);
    OPENSSL_free(publicBIO);
    free(publicKeyStr);
}

int COMiC_Network_Buffer_read_packet_id(COMiC_Network_ByteBuffer *buf)
{
    return COMiC_Network_Buffer_read_var_int(buf);
}

void COMiC_Network_Buffer_write_packet_id(COMiC_Network_ByteBuffer *buf, COMiC_Network_PacketID id)
{
    COMiC_Network_Buffer_write_var_int(buf, (int) id & 0xFF);
}
