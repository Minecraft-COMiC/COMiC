#include "buffer.h"

ByteBuffer network_buffer_new(unsigned int capacity)
{
    return (ByteBuffer) {malloc(capacity), 5, capacity};
}

ByteBuffer network_buffer_new_with_default_capacity()
{
    return network_buffer_new(100);
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

void network_buffer_prepare(ByteBuffer *buf)
{
    buf->index -= 5;
    unsigned int data_size = buf->index;

    unsigned int offset = get_var_int_size_in_bytes((int) buf->index);
    buf->index = 5 - offset;
    network_buffer_write_var_int(buf, (int) data_size);
    buf->bytes += (5 - offset);

    buf->size = data_size + offset;
}

void network_buffer_skip_bytes(ByteBuffer *buf, unsigned int count)
{
    buf->index = buf->index + count;
}

char network_buffer_read_byte(ByteBuffer *buf)
{
    return buf->bytes[buf->index++];
}

void network_buffer_write_byte(ByteBuffer *buf, char byte)
{
    if (buf->index + 1 > buf->size)
    {
        buf->bytes = realloc(buf->bytes, buf->size * 2);
        buf->size *= 2;
    }

    buf->bytes[buf->index++] = byte;
}

int network_buffer_read_var_int(ByteBuffer *buf)
{
    int res = 0;
    char byte;

    for (int i = 0; i < 6; i++)
    {
        byte = network_buffer_read_byte(buf);
        res |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return res;
}

void network_buffer_write_var_int(ByteBuffer *buf, int value)
{
    for (int i = 0; i < 6; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            network_buffer_write_byte(buf, (char) value);
            return;
        }

        network_buffer_write_byte(buf, (char) ((value & 0x7F) | 0x80));
        value = (int) ((unsigned int) value >> 7);
    }
}

long long network_buffer_read_var_long(ByteBuffer *buf)
{
    long long value = 0;
    char byte;

    for (int i = 0; i < 11; i++)
    {
        byte = network_buffer_read_byte(buf);
        value |= (byte & 0x7F) << (i * 7);

        if ((byte & 0x80) != 0x80) break;
    }

    return value;
}

void network_buffer_write_var_long(ByteBuffer *buf, long long value)
{
    for (int i = 0; i < 11; i++)
    {
        if ((value & ~0x7F) == 0)
        {
            network_buffer_write_byte(buf, (char) value);
            return;
        }

        network_buffer_write_byte(buf, (char) ((value & 0x7F) | 0x80));
        value = (long long) ((unsigned long long) value >> 7);
    }
}

int network_buffer_read_bool(ByteBuffer *buf)
{
    return network_buffer_read_byte(buf) != 0;
}

void network_buffer_write_bool(ByteBuffer *buf, int value)
{
    network_buffer_write_byte(buf, (char) (value != 0));
}

short network_buffer_read_short(ByteBuffer *buf)
{
    return (short) ((network_buffer_read_byte(buf) & 0xFF) << 8 | network_buffer_read_byte(buf) & 0xFF);
}

void network_buffer_write_short(ByteBuffer *buf, short value)
{
    network_buffer_write_byte(buf, (char) ((unsigned short) value >> 8));
    network_buffer_write_byte(buf, (char) value);
}

int network_buffer_read_int(ByteBuffer *buf)
{
    return (network_buffer_read_short(buf) & 0xFFFF) << 16 | network_buffer_read_short(buf) & 0xFFFF;
}

void network_buffer_write_int(ByteBuffer *buf, int value)
{
    network_buffer_write_short(buf, (short) ((unsigned int) value >> 16));
    network_buffer_write_short(buf, (short) value);
}

long long network_buffer_read_long(ByteBuffer *buf)
{
    return (long long) (network_buffer_read_int(buf) & 0xFFFFFFFFL) << 32 | network_buffer_read_int(buf) & 0xFFFFFFFFL;
}

void network_buffer_write_long(ByteBuffer *buf, long long value)
{
    network_buffer_write_int(buf, (int) ((unsigned long long) value >> 32));
    network_buffer_write_int(buf, (int) value);
}

float network_buffer_read_float(ByteBuffer *buf)
{
    int v = network_buffer_read_int(buf);

    union
    {
        int i;
        float f;
    } u;
    u.i = v;

    return u.f;
}

void network_buffer_write_float(ByteBuffer *buf, float value)
{
    union
    {
        int i;
        float f;
    } u;
    u.f = value;

    network_buffer_write_int(buf, u.i);
}

double network_buffer_read_double(ByteBuffer *buf)
{
    long long v = network_buffer_read_long(buf);

    union
    {
        long long l;
        double d;
    } u;
    u.l = v;

    return u.d;
}

void network_buffer_write_double(ByteBuffer *buf, double value)
{
    union
    {
        long long l;
        double d;
    } u;
    u.d = value;

    network_buffer_write_long(buf, u.l);
}

void network_buffer_read_string(ByteBuffer *buf, char *out, unsigned int max_length)
{
    int str_len = network_buffer_read_var_int(buf);

    if (str_len > max_length * 4)
        return;

    memcpy(out, buf->bytes + buf->index, str_len);
    out[str_len] = 0;

    network_buffer_skip_bytes(buf, str_len);
}

void network_buffer_write_string(ByteBuffer *buf, char *str, unsigned int max_length)
{
    unsigned int str_len = strlen(str);

    if (str_len > max_length) return;

    network_buffer_write_var_int(buf, (int) str_len);

    for (int j = 0; j < str_len; j++)
        network_buffer_write_byte(buf, str[j]);
}

int network_buffer_read_enum(ByteBuffer *buf)
{
    return network_buffer_read_var_int(buf);
}

void network_buffer_write_enum(ByteBuffer *buf, int value)
{
    network_buffer_write_var_int(buf, value);
}

void network_buffer_read_byte_array(ByteBuffer *buf, char *out, unsigned int count)
{
    memcpy(out, buf->bytes + buf->index, count);

    network_buffer_skip_bytes(buf, count);
}

void network_buffer_write_byte_array(ByteBuffer *buf, char *arr, unsigned int count)
{
    network_buffer_write_var_int(buf, (int) count);

    for (int i = 0; i < count; i++)
        network_buffer_write_byte(buf, arr[i]);
}

void network_buffer_write_rsa_key(ByteBuffer *buf, EVP_PKEY *key)
{
    BIO *publicBIO = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(publicBIO, key);
    int publicKeyLen = BIO_pending(publicBIO);
    char *publicKeyStr = (char *) malloc(publicKeyLen);
    BIO_read(publicBIO, publicKeyStr, publicKeyLen);

    network_buffer_write_byte_array(buf, publicKeyStr, publicKeyLen);
    OPENSSL_free(publicBIO);
    free(publicKeyStr);
}

int network_buffer_read_packet_id(ByteBuffer *buf)
{
    return network_buffer_read_var_int(buf);
}

void network_buffer_write_packet_id(ByteBuffer *buf, PacketID id)
{
    network_buffer_write_var_int(buf, (int) id & 0xFF);
}
