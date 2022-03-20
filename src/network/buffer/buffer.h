#ifndef COMIC_BUFFER_H
#define COMIC_BUFFER_H

#include <string.h>
#include <stdlib.h>
#include "../../util/util.h"
#include "../network_constants.h"

typedef struct COMiC_Network_ByteBuffer
{
    char *bytes;
    unsigned int index;
    unsigned int size;
} COMiC_Network_ByteBuffer;

COMiC_Network_ByteBuffer COMiC_Network_Buffer_new(unsigned int);

COMiC_Network_ByteBuffer COMiC_Network_Buffer_new_with_default_capacity(void);

void COMiC_Network_Buffer_prepare(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_skip_bytes(COMiC_Network_ByteBuffer *buf, unsigned int count);

char COMiC_Network_Buffer_read_byte(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_byte(COMiC_Network_ByteBuffer *buf, char byte);

int COMiC_Network_Buffer_read_var_int(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_var_int(COMiC_Network_ByteBuffer *buf, int value);

long long COMiC_Network_Buffer_read_var_long(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_var_long(COMiC_Network_ByteBuffer *buf, long long value);

int COMiC_Network_Buffer_read_bool(COMiC_Network_ByteBuffer *buf);

void COMiC_network_buffer_write_bool(COMiC_Network_ByteBuffer *buf, int value);

short COMiC_Network_Buffer_read_short(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_short(COMiC_Network_ByteBuffer *buf, short value);

int COMiC_Network_Buffer_read_int(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_int(COMiC_Network_ByteBuffer *buf, int value);

long long COMiC_Network_Buffer_read_long(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_long(COMiC_Network_ByteBuffer *buf, long long value);

float COMiC_Network_Buffer_read_float(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_float(COMiC_Network_ByteBuffer *buf, float value);

double COMiC_Network_Buffer_read_double(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_double(COMiC_Network_ByteBuffer *buf, double value);

void COMiC_Network_Buffer_read_string(COMiC_Network_ByteBuffer *buf, char *out, unsigned int max_length);

void COMiC_Network_Buffer_write_string(COMiC_Network_ByteBuffer *buf, char *str, unsigned int max_length);

char *COMiC_Network_Buffer_read_identifier(COMiC_Network_ByteBuffer *);

void COMiC_Network_Buffer_write_identifier(COMiC_Network_ByteBuffer *, char *, char *);

int COMiC_Network_Buffer_read_enum(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_enum(COMiC_Network_ByteBuffer *buf, int value);

void COMiC_Network_Buffer_read_byte_array(COMiC_Network_ByteBuffer *buf, char *out, unsigned int count);

void COMiC_Network_Buffer_write_byte_array(COMiC_Network_ByteBuffer *buf, char *arr, unsigned int count);

void COMiC_Network_Buffer_write_rsa_key(COMiC_Network_ByteBuffer *buf, EVP_PKEY *key);

int COMiC_Network_Buffer_read_packet_id(COMiC_Network_ByteBuffer *buf);

void COMiC_Network_Buffer_write_packet_id(COMiC_Network_ByteBuffer *buf, COMiC_Network_PacketID id);

#endif //COMIC_BUFFER_H
