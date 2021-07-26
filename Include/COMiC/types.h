#include <stdint.h>

#ifndef COMiC_TYPES_H
# define COMiC_TYPES_H

# ifndef COMiC_LIMITED
#  ifndef COMiC_FAST
#   define COMiC_FAST
#  endif
# endif


# ifdef COMiC_LIMITED
typedef uint_least64_t COMiC_block_id_t;
# else
typedef uint_fast64_t COMiC_block_id_t;
# endif


# ifdef COMiC_LIMITED
#  pragma pack(push, 1)
# endif
struct COMiC_block_render_attributes
{
# ifdef COMiC_LIMITED
    uint_least8_t flags;
    uint_least8_t light;
    uint_least8_t render;
# else
    uint_fast8_t flags;
    uint_fast8_t light;
    uint_fast8_t render;
# endif
};
# ifdef COMiC_LIMITED
#  pargma pop()
# endif
# define COMiC_block_render_attributes_TRANSPARENT (0x01)
# define COMiC_block_render_attributes_FULL (0x02)


# ifdef COMiC_LIMITED
typedef uint_least32_t COMiC_block_data_index_t;
# else
typedef uint_fast32_t COMiC_block_data_index_t;
# endif


# ifdef COMiC_LIMITED
#  pragma pack(push, 1)
# endif
struct COMiC_block
{
    COMiC_block_id_t id;
    struct COMiC_block_render_attributes ra;
    COMiC_block_data_index_t data_index;
};
typedef struct COMiC_block COMiC_block;
# ifdef COMiC_LIMITED
#  pargma pop()
# endif


# ifdef COMiC_LIMITED
typedef uint_least32_t COMiC_coordinate_t;
# else
typedef uint_fast32_t COMiC_coordinate_t;
# endif


#define COMiC_chunk_X_SIZE (16)
#define COMiC_chunk_Y_SIZE (256)
#define COMiC_chunk_Z_SIZE (16)
typedef struct COMiC_block COMiC_chunk_blocks_array[COMiC_chunk_X_SIZE][COMiC_chunk_Y_SIZE][COMiC_chunk_Z_SIZE];
# ifdef COMiC_LIMITED
#  pragma pack(push, 1)
# endif
struct COMiC_chunk
{
    COMiC_coordinate_t x;
    COMiC_coordinate_t y;
    COMiC_coordinate_t z;
    COMiC_chunk_blocks_array blocks;
    void *blocks_data;
};
typedef struct COMiC_chunk COMiC_chunk;
# ifdef COMiC_LIMITED
#  pargma pop()
# endif


#define COMiC_region_X_SIZE (32)
#define COMiC_region_Z_SIZE (32)
typedef struct COMiC_chunk COMiC_region_chunks_array[COMiC_region_X_SIZE][COMiC_region_Z_SIZE];
# ifdef COMiC_LIMITED
#  pragma pack(push, 1)
# endif
struct COMiC_region
{
    COMiC_coordinate_t x;
    COMiC_coordinate_t z;
    COMiC_region_chunks_array chunks;
    void *blocks_data_buffer;
};
typedef struct COMiC_region COMiC_region;
# ifdef COMiC_LIMITED
#  pargma pop()
# endif

#endif /* COMiC_TYPES_H */
