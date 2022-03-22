#include <cstring>

#include <COMiC.h>
#include "chunk_extra_buffer.h"


template<typename page_t>
static inline COMiC_IfError _COMiC_ChunkExtraBuffer_Push(
        COMiC_In struct COMiC_ChunkExtraBuffer_Buffer_Meta *self,
        COMiC_In COMiC_Heap *heap,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
)
{
    struct COMiC_ChunkExtraBuffer_Page_Meta *page;
    COMiC_USize pages_required = -(-(size + sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta)) / sizeof(page_t));

    if (self->buffer == NULL)
    {
        goto extend;
    }

    for (page = self->first_free_block; page != NULL; page = page->next)
    {
        if (page->is_free && page->block_size >= pages_required)
        {
            goto found;
        }
    }

    extend:
    {
        struct COMiC_ChunkExtraBuffer_Head *new_buffer;
        COMiC_USize new_buffer_size;

        new_buffer_size = self->pages_count == 0 ? 4 : self->pages_count;
        if (COMiC_Alloc(
                heap,
                error,
                (char *) "Can't extend chunk extra buffer; May be not enough memory",
                (void **) (&new_buffer),
                sizeof(struct COMiC_ChunkExtraBuffer_Head) + new_buffer_size * sizeof(page_t)
        ))
        {

            return COMiC_ERROR;
        }

        new_buffer->next = self->buffer;
        new_buffer->owner = (struct COMiC_ChunkExtraBuffer_Buffer_Meta *) (self);
        new_buffer->capacity = new_buffer->used = new_buffer_size;
        self->buffer = new_buffer;

        page = (struct COMiC_ChunkExtraBuffer_Page_Meta *) (((COMiC_UIntPtr) (new_buffer)) + sizeof(COMiC_ChunkExtraBuffer_Head));
        COMiC_ChunkExtraBuffer_Page_InitMeta(
                page,
                new_buffer_size,
                new_buffer,
                COMiC_TRUE,
                NULL,
                self->last_free_block
        );

        if (self->last_free_block == NULL)
        {
            self->last_free_block = self->first_free_block = page;
        }
        else
        {
            self->last_free_block = self->last_free_block->next = page;
        }
    }

    found:

    page->block_size -= pages_required;
    if (page->block_size == 0)
    {
        if (page->prev != NULL)
        {
            page->prev->next = page->next;
        }
        if (page->next != NULL)
        {
            page->next->prev = page->prev;
        }
        COMiC_ChunkExtraBuffer_Page_InitMeta(
                page,
                pages_required,
                page->owner,
                COMiC_FALSE,
                NULL,
                page->prev
        );
    }
    else
    {
        *stored = (void *) (((page_t *) (page)) + page->block_size);
        COMiC_ChunkExtraBuffer_Page_InitMeta(
                (struct COMiC_ChunkExtraBuffer_Page_Meta *) (*stored),
                pages_required,
                page->owner,
                COMiC_FALSE,
                NULL,
                page
        );
    }

    *stored = (void *) (((COMiC_USize) (*stored)) + sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta));
    page->owner->used -= pages_required;
    page->owner->owner->free_pages_count -= pages_required;
    memcpy(*stored, data, size);

    return COMiC_SUCCESS;
}

inline COMiC_IfError COMiC_ChunkExtraBuffer_PushSmall(
        COMiC_In struct COMiC_ChunkExtraBuffer_Buffer_Meta *self,
        COMiC_In COMiC_Heap *heap,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
)
{
    return _COMiC_ChunkExtraBuffer_Push<union _COMiC_ChunkExtraBuffer_SmallPage>(
            self,
            heap,
            error,
            size,
            data,
            stored
    );
}

inline COMiC_IfError COMiC_ChunkExtraBuffer_PushBig(
        COMiC_In struct COMiC_ChunkExtraBuffer_Buffer_Meta *self,
        COMiC_In COMiC_Heap *heap,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_USize size,
        COMiC_In void *data,
        COMiC_Out void **stored
)
{
    return _COMiC_ChunkExtraBuffer_Push<union _COMiC_ChunkExtraBuffer_BigPage>(
            self,
            heap,
            error,
            size,
            data,
            stored
    );
}