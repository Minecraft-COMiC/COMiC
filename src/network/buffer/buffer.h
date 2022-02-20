#ifndef COMIC_BUFFER_H
#define COMIC_BUFFER_H

#include <string.h>
#include <stdlib.h>
#include "../../util/util.h"
#include "../network_constants.h"

typedef struct ByteBuffer
{
    char *bytes;
    unsigned int index;
    unsigned int size;
} ByteBuffer;

ByteBuffer network_buffer_new(unsigned int);

ByteBuffer network_buffer_new_with_default_capacity(void);

void network_buffer_prepare(ByteBuffer *buf);

void network_buffer_skip_bytes(ByteBuffer *buf, unsigned int count);

char network_buffer_read_byte(ByteBuffer *buf);

void network_buffer_write_byte(ByteBuffer *buf, char byte);

int network_buffer_read_var_int(ByteBuffer *buf);

void network_buffer_write_var_int(ByteBuffer *buf, int value);

long long network_buffer_read_var_long(ByteBuffer *buf);

void network_buffer_write_var_long(ByteBuffer *buf, long long value);

int network_buffer_read_bool(ByteBuffer *buf);

void network_buffer_write_bool(ByteBuffer *buf, int value);

short network_buffer_read_short(ByteBuffer *buf);

void network_buffer_write_short(ByteBuffer *buf, short value);

int network_buffer_read_int(ByteBuffer *buf);

void network_buffer_write_int(ByteBuffer *buf, int value);

long long network_buffer_read_long(ByteBuffer *buf);

void network_buffer_write_long(ByteBuffer *buf, long long value);

float network_buffer_read_float(ByteBuffer *buf);

void network_buffer_write_float(ByteBuffer *buf, float value);

double network_buffer_read_double(ByteBuffer *buf);

void network_buffer_write_double(ByteBuffer *buf, double value);

void network_buffer_read_string(ByteBuffer *buf, char *out, unsigned int max_length);

void network_buffer_write_string(ByteBuffer *buf, char *str, unsigned int max_length);

char *readIdentifier(ByteBuffer *);

void writeIdentifier(ByteBuffer *, char *, char *);

int network_buffer_read_enum(ByteBuffer *buf);

void network_buffer_write_enum(ByteBuffer *buf, int value);

void network_buffer_read_byte_array(ByteBuffer *buf, char *out, unsigned int count);

void network_buffer_write_byte_array(ByteBuffer *buf, char *arr, unsigned int count);

void network_buffer_write_rsa_key(ByteBuffer *buf, EVP_PKEY *key);

int network_buffer_read_packet_id(ByteBuffer *buf);

void network_buffer_write_packet_id(ByteBuffer *buf, PacketID id);

#endif //COMIC_BUFFER_H
