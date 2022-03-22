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
    COMiC_USize pages_count;
    COMiC_USize free_pages_count;
    COMiC_USize first_unused_block;
    COMiC_USize last_unused_block;
    COMiC_Heap *heap;
    COMiC_Optional(NULL) struct COMiC_ChunkExtraBuffer_Head *next;
};

union COMiC_ChunkExtraBuffer_Page
{
    COMiC_Byte data[16 * sizeof(COMiC_UIntPtr)];
    struct COMiC_ChunkExtraBuffer_Page_Meta
    {
        COMiC_USize block_size;
        COMiC_Optional(0) COMiC_USize next_unused_index;
        COMiC_Optional((COMiC_USize) -1) COMiC_USize prev_unused_index;
        COMiC_Optional((COMiC_USize) -1) COMiC_USize prev_used_index;
    } meta;
};

COMiC_Constructor
static inline void COMiC_ChunkExtraBuffer_Page_InitMeta(
        COMiC_Out union COMiC_ChunkExtraBuffer_Page *self,
        COMiC_In COMiC_USize sequence_size,
        COMiC_In COMiC_Optional(0) COMiC_USize next_unused_index,
        COMiC_In COMiC_Optional((COMiC_USize) -1) COMiC_USize prev_unused_index,
        COMiC_In COMiC_Optional((COMiC_USize) -1) COMiC_USize prev_used_index
)
{
    self->meta.block_size = sequence_size;
    self->meta.next_unused_index = next_unused_index;
    self->meta.prev_unused_index = prev_unused_index;
    self->meta.prev_used_index = prev_used_index;
}


# define COMiC_ChunkExtraBuffer_Page_Size (sizeof(union COMiC_ChunkExtraBuffer_Page))

# define COMiC_ChunkExtraBuffer_GetPage(HEAD_P, INDEX) ((union COMiC_ChunkExtraBuffer_Page *)((COMiC_UIntPtr)(HEAD_P) + sizeof(struct COMiC_ChunkExtraBuffer_Head) + COMiC_ChunkExtraBuffer_Page_Size * (INDEX)))

COMiC_Constructor
COMiC_IfError COMiC_ChunkExtraBuffer_Create(
        COMiC_Out struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Heap *heap,
        COMiC_In COMiC_USize capacity
);

COMiC_Destructor
COMiC_IfError COMiC_ChunkExtraBuffer_Destroy(
        COMiC_In struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize capacity
);

COMiC_IfError COMiC_ChunkExtraBuffer_Push(
        COMiC_In struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
);

COMiC_IfError COMiC_ChunkExtraBuffer_Pop(
        COMiC_In struct COMiC_ChunkExtraBuffer_Head **self,
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
    struct COMiC_ChunkExtraBuffer_Head *buffer;
} COMiC_Chunk;

# ifdef __cplusplus
};
# endif

#endif /* COMiC_WORLD_H */