#include <string.h>

#include <COMiC.h>

COMiC_Constructor
COMiC_IfError COMiC_ChunkExtraBuffer_Create(
        COMiC_Out struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Heap *heap,
        COMiC_In COMiC_USize capacity
)
{
    capacity = -(-capacity / COMiC_ChunkExtraBuffer_Page_Size);

    if (COMiC_Alloc(
            heap,
            error,
            "Can't alloc block state buffer for chunk; May be not enough memory",
            (void **) self,
            sizeof(struct COMiC_ChunkExtraBuffer_Head) + capacity * COMiC_ChunkExtraBuffer_Page_Size
    ))
    {
        return COMiC_ERROR;
    }
    (*self)->heap = heap;
    (*self)->next = NULL;
    (*self)->pages_count = capacity;
    (*self)->free_pages_count = capacity;
    (*self)->first_unused_block = 0;
    (*self)->last_unused_block = 0;

    COMiC_ChunkExtraBuffer_Page_InitMeta(
            COMiC_ChunkExtraBuffer_GetPage(*self, 0),
            capacity,
            0,
            (COMiC_USize) -1,
            (COMiC_USize) -1
    );

    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_ChunkExtraBuffer_Destroy(
        COMiC_In struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize capacity
)
{
    COMiC_In struct COMiC_ChunkExtraBuffer_Head *next = *self;
    while (*self != NULL)
    {
        next = (*self)->next;
        if (COMiC_Free(
                (*self)->heap,
                error,
                (void **) self
        ))
        {
            return COMiC_ERROR;
        }
        *self = next;
    }
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_ChunkExtraBuffer_Push(
        COMiC_In struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
)
{
    COMiC_USize required_pages = -(-(size + sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta)) / COMiC_ChunkExtraBuffer_Page_Size);
    struct COMiC_ChunkExtraBuffer_Head *buffer = *self;
    COMiC_USize pos = buffer->first_unused_block;
    union COMiC_ChunkExtraBuffer_Page *page;
    union COMiC_ChunkExtraBuffer_Page *next_page;
    COMiC_USize next_pos;

    do
    {
        if (required_pages > buffer->free_pages_count)
        {
            continue;
        }

        while (pos < buffer->pages_count)
        {
            page = COMiC_ChunkExtraBuffer_GetPage(buffer, pos);
            if (page->meta.block_size >= required_pages)
            {
                goto found;
            }
            pos = page->meta.next_unused_index;
        }
    } while ((buffer->next != NULL) && (buffer == buffer->next));

    if (COMiC_ChunkExtraBuffer_Create(
            &(buffer->next),
            error,
            buffer->heap,
            (required_pages > (buffer->pages_count << 1)) ? required_pages : (buffer->pages_count << 1)
    ))
    {
        return COMiC_ERROR;
    }
    buffer = buffer->next;
    page = COMiC_ChunkExtraBuffer_GetPage(buffer, 0);

    found:

    if (required_pages == page->meta.block_size)
    {
        next_pos = page->meta.next_unused_index;
        if (next_pos > pos)
        {
            next_page = COMiC_ChunkExtraBuffer_GetPage(buffer, next_pos);
            next_page->meta.prev_unused_index = page->meta.prev_unused_index;
        }
    }
    else
    {
        next_pos = pos + required_pages;
        next_page = COMiC_ChunkExtraBuffer_GetPage(buffer, next_pos);
        COMiC_ChunkExtraBuffer_Page_InitMeta(
                next_page,
                page->meta.block_size - required_pages,
                page->meta.next_unused_index,
                page->meta.prev_unused_index,
                pos
        );
        if (next_page->meta.next_unused_index > next_pos)
        {
            next_page = COMiC_ChunkExtraBuffer_GetPage(buffer, next_page->meta.next_unused_index);
            next_page->meta.prev_unused_index = next_pos;
        }
    }

    COMiC_ChunkExtraBuffer_Page_InitMeta(
            page,
            page->meta.block_size - required_pages,
            page->meta.next_unused_index,
            page->meta.prev_unused_index,
            pos
    );

    *stored = (void *) (((COMiC_UIntPtr) (page)) + sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta));
    memcpy(*stored, data, size);

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_ChunkExtraBuffer_Pop(
        COMiC_In struct COMiC_ChunkExtraBuffer_Head **self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **stored
)
{

}