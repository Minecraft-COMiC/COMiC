#ifndef COMiC__CHUNK_EXTRA_BUFFER_H
# define COMiC__CHUNK_EXTRA_BUFFER_H

# include <COMiC.h>

# ifdef __cplusplus
extern "C" {
# endif


COMiC_Constructor
inline void COMiC_ChunkExtraBuffer_Page_InitMeta(
        COMiC_Out struct COMiC_ChunkExtraBuffer_Page_Meta *self,
        COMiC_In COMiC_USize block_size,
        COMiC_In struct COMiC_ChunkExtraBuffer_Head *owner,
        COMiC_In COMiC_Bool is_free,
        COMiC_In COMiC_Optional(NULL) struct COMiC_ChunkExtraBuffer_Page_Meta *next,
        COMiC_In COMiC_Optional(NULL) struct COMiC_ChunkExtraBuffer_Page_Meta *prev
);

union _COMiC_ChunkExtraBuffer_SmallPage
{
    struct COMiC_ChunkExtraBuffer_Page_Meta meta;
    COMiC_Byte data[16 * sizeof(void *)];
};

union _COMiC_ChunkExtraBuffer_BigPage
{
    struct COMiC_ChunkExtraBuffer_Page_Meta meta;
    COMiC_Byte data[sizeof(union _COMiC_ChunkExtraBuffer_SmallPage) * 4];
};


inline COMiC_IfError COMiC_ChunkExtraBuffer_PushSmall(
        COMiC_In struct COMiC_ChunkExtraBuffer_Buffer_Meta *self,
        COMiC_In COMiC_Heap *heap,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
);

inline COMiC_IfError COMiC_ChunkExtraBuffer_PushBig(
        COMiC_In struct COMiC_ChunkExtraBuffer_Buffer_Meta *self,
        COMiC_In COMiC_Heap *heap,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
);

# ifdef __cplusplus
};
# endif

#endif /* COMiC__CHUNK_EXTRA_BUFFER_H */