#ifndef COMiC_WORLD_H
# define COMiC_WORLD_H

# include <COMiC/types.h>

# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_BlockMeta
{
    void *something;
    /* todo */
} COMiC_BlockMeta;

typedef struct COMiC_ItemMeta
{
    void *something;
    /* todo */
} COMiC_ItemMeta;

typedef struct COMiC_Biome
{
    void *something;
    /* todo */
} COMiC_Biome;

typedef struct COMiC_Block
{
    COMiC_BlockMeta *meta;
    COMiC_Optional(0) COMiC_UInt32 state;
    COMiC_Optional(NULL) void *extra_data;
} COMiC_Block;

typedef struct COMiC_Item
{
    COMiC_ItemMeta *meta;
    COMiC_Optional(0) COMiC_UInt32 state;
    COMiC_Optional(NULL) void *extra_data;
} COMiC_Item;

typedef struct COMiC_ItemStack
{
    COMiC_Block block;
    COMiC_UInt8 count;
} COMiC_ItemStack;

typedef struct COMiC_Chunk_Block
{
    COMiC_Biome *biome;
    COMiC_Block block;
} COMiC_Chunk_Block;

struct COMiC_Chunk_Pie
{
    struct COMiC_Chunk_Block array[16][16];
};

struct COMiC_ChunkExtraBuffer_Head
{
    struct COMiC_ChunkExtraBuffer_Head *next;
    struct COMiC_ChunkExtraBuffer_Buffer_Meta *owner;
    COMiC_USize used;
    COMiC_USize capacity;
};

struct COMiC_ChunkExtraBuffer_Page_Meta
{
    COMiC_USize block_size;
    struct COMiC_ChunkExtraBuffer_Head *owner;
    COMiC_Bool is_free;
    struct COMiC_ChunkExtraBuffer_Page_Meta *next;
    struct COMiC_ChunkExtraBuffer_Page_Meta *prev;
};

struct COMiC_ChunkExtraBuffer_Buffer_Meta
{
    COMiC_USize pages_count;
    COMiC_USize free_pages_count;
    struct COMiC_ChunkExtraBuffer_Page_Meta *first_free_block;
    struct COMiC_ChunkExtraBuffer_Page_Meta *last_free_block;
    struct COMiC_ChunkExtraBuffer_Head *buffer;
};

typedef struct COMiC_ChunkExtraBuffer
{
    COMiC_Heap *heap;
    struct COMiC_ChunkExtraBuffer_Buffer_Meta small_pages;
    struct COMiC_ChunkExtraBuffer_Buffer_Meta big_pages;
} COMiC_ChunkExtraBuffer;

COMiC_Constructor
COMiC_IfError COMiC_ChunkExtraBuffer_Init(
        COMiC_Out COMiC_ChunkExtraBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Heap *heap
);

COMiC_Destructor
COMiC_IfError COMiC_ChunkExtraBuffer_Finalize(
        COMiC_In COMiC_ChunkExtraBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize capacity
);

COMiC_IfError COMiC_ChunkExtraBuffer_Push(
        COMiC_In COMiC_ChunkExtraBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
);

COMiC_IfError COMiC_ChunkExtraBuffer_Pop(
        COMiC_In COMiC_ChunkExtraBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **stored
);

typedef struct COMiC_Chunk_Head
{
    COMiC_Optional(NULL) struct COMiC_Chunk_Head *north;
    COMiC_Optional(NULL) struct COMiC_Chunk_Head *west;
    COMiC_Optional(NULL) struct COMiC_Chunk_Head *south;
    COMiC_Optional(NULL) struct COMiC_Chunk_Head *east;
    COMiC_UInt16 height;
    COMiC_ChunkExtraBuffer buffer;
} COMiC_Chunk;

# ifdef __cplusplus
};
# endif

#endif /* COMiC_WORLD_H */