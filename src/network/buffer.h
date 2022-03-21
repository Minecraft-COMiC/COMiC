#ifndef COMIC_BUFFER_H
#define COMIC_BUFFER_H

#include <string.h>
#include <stdlib.h>
#include <COMiC/util.h>
#include <COMiC/types.h>

typedef struct COMiC_Network_ByteBuffer
{
    char *bytes;
    unsigned int index;
    unsigned int size;
} COMiC_Network_ByteBuffer;

COMiC_Constructor
COMiC_Network_ByteBuffer COMiC_Network_Buffer_Init(
        COMiC_In unsigned int capacity
);

COMiC_Constructor
COMiC_Network_ByteBuffer COMiC_Network_Buffer_InitWithDefaultCapacity(void);

void COMiC_Network_Buffer_Prepare(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_SkipBytes(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In unsigned int count
);

char COMiC_Network_Buffer_ReadByte(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteByte(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In char byte
);

int COMiC_Network_Buffer_ReadVarInt(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteVarInt(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
);

long long COMiC_Network_Buffer_ReadVarLong(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteVarLong(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In long long value
);

COMiC_Bool COMiC_Network_Buffer_ReadBool(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteBool(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
);

short COMiC_Network_Buffer_ReadShort(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteShort(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In short value
);

int COMiC_Network_Buffer_ReadInt(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteInt(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
);

long long COMiC_Network_Buffer_ReadLong(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteLong(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In long long value
);

float COMiC_Network_Buffer_ReadFloat(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteFloat(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In float value
);

double COMiC_Network_Buffer_ReadDouble(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteDouble(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In double value
);

void COMiC_Network_Buffer_ReadString(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_Out char *out,
        COMiC_In unsigned int max_length
);

void COMiC_Network_Buffer_WriteString(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In char *str,
        COMiC_In unsigned int max_length
);

int COMiC_Network_Buffer_ReadEnum(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WriteEnum(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int value
);

void COMiC_Network_Buffer_ReadByteArray(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_Out char *out,
        COMiC_In unsigned int count
);

void COMiC_Network_Buffer_WriteByteArray(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In char *arr,
        COMiC_In unsigned int count
);

void COMiC_Network_Buffer_WriteRSAKey(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In EVP_PKEY *key
);

int COMiC_Network_Buffer_ReadPacketID(
        COMiC_Out COMiC_Network_ByteBuffer *self
);

void COMiC_Network_Buffer_WritePacketID(
        COMiC_Out COMiC_Network_ByteBuffer *self,
        COMiC_In int id
);

#endif //COMIC_BUFFER_H
