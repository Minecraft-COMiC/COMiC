#include <COMiC.h>
#include "chunk_extra_buffer.h"


COMiC_Constructor
inline void COMiC_ChunkExtraBuffer_Page_InitMeta(
        COMiC_Out struct COMiC_ChunkExtraBuffer_Page_Meta *self,
        COMiC_In COMiC_USize block_size,
        COMiC_In struct COMiC_ChunkExtraBuffer_Head *owner,
        COMiC_In COMiC_Bool is_free,
        COMiC_In COMiC_Optional(NULL) struct COMiC_ChunkExtraBuffer_Page_Meta *next,
        COMiC_In COMiC_Optional(NULL) struct COMiC_ChunkExtraBuffer_Page_Meta *prev
)
{
    self->block_size = block_size;
    self->owner = owner;
    self->is_free = is_free;
    self->next = next;
    self->prev = prev;
}

COMiC_Constructor
COMiC_IfError COMiC_ChunkExtraBuffer_Init(
        COMiC_Out COMiC_ChunkExtraBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Heap *heap
)
{
    self->heap = heap;
    self->small_pages.pages_count = 0;
    self->small_pages.free_pages_count = 0;
    self->small_pages.first_free_block = NULL;
    self->small_pages.last_free_block = NULL;
    self->small_pages.buffer = NULL;
    self->big_pages.pages_count = 0;
    self->big_pages.free_pages_count = 0;
    self->big_pages.first_free_block = NULL;
    self->big_pages.last_free_block = NULL;
    self->big_pages.buffer = NULL;
    return COMiC_SUCCESS;
}

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
)
{
    if (size == 0)
    {
        return COMiC_Error_Set(
                error,
                COMiC_ErrNo_EmptyAlloc,
                (char *) "Empty request to chunk extra buffer",
                NULL,
                COMiC_Error_NoDealloc
        );
    }
    if (size + sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta) > sizeof(union _COMiC_ChunkExtraBuffer_SmallPage))
    {
        return COMiC_ChunkExtraBuffer_PushBig(
                &(self->big_pages),
                self->heap,
                error,
                size,
                data,
                stored
        );
    }
    else
    {
        return COMiC_ChunkExtraBuffer_PushSmall(
                &(self->big_pages),
                self->heap,
                error,
                size,
                data,
                stored
        );
    }
}

COMiC_IfError COMiC_ChunkExtraBuffer_Pop(
        COMiC_In COMiC_ChunkExtraBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **stored
)
{
    struct COMiC_ChunkExtraBuffer_Page_Meta *page = (struct COMiC_ChunkExtraBuffer_Page_Meta *) (((COMiC_UIntPtr) (*stored)) - sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta));
    struct COMiC_ChunkExtraBuffer_Head *buffer;
    struct COMiC_ChunkExtraBuffer_Head *next_buffer;
    struct COMiC_ChunkExtraBuffer_Buffer_Meta *meta = page->owner->owner;
    struct COMiC_ChunkExtraBuffer_Page_Meta *free_page;
    register COMiC_Bool before_page = COMiC_TRUE;

    if (meta != &(self->small_pages) && meta != &(self->big_pages))
    {
        return COMiC_Error_Set(
                error,
                COMiC_ErrNo_FreeInvalidPointer,
                "Pointer doesn't correspond to specified chunk extra buffer (owned by another buffer)",
                NULL,
                COMiC_Error_NoDealloc
        );
    }

    free_page = meta->first_free_block;
    if (free_page == NULL)
    {
        meta->first_free_block = meta->last_free_block = page;
        goto resolve;
    }

    buffer = free_page->owner;
    while (buffer != free_page->owner)
    {
        if (buffer == NULL)
        {
            return COMiC_Error_Set(
                    error,
                    COMiC_ErrNo_FreeInvalidPointer,
                    "Pointer doesn't correspond to specified chunk extra buffer (owner not linked)",
                    NULL,
                    COMiC_Error_NoDealloc
            );
        }
        if (buffer == page->owner)
        {

            buffer = buffer->next;
        }

    }

    for (buffer = free_page->owner; buffer != NULL; buffer = buffer->next)
    {
        if (free_page->owner == page->owner)
        {
            if (free_page->next == NULL)
            {
                goto resolve;
            }
        }
        next_buffer = free_page->next->owner;
        if (buffer == page->owner)
        {
            if (before_page)
            {
                page->next = free_page
                goto resolve;
            }
            else
            { goto align; }
        }
        while (free_page->owner == buffer)
        {
            if (free_page->next == NULL)
            { goto resolve; }
            free_page = free_page->next;
            before_page = COMiC_FALSE;
        }
    }
    return COMiC_Error_Set(
            error,
            COMiC_ErrNo_FreeInvalidPointer,
            "Pointer doesn't correspond to specified chunk extra buffer (owner not linked)",
            NULL,
            COMiC_Error_NoDealloc
    );
    align:
    while (free_page->next != NULL)
    {
        if (free_page->next->owner == buffer && ((COMiC_UIntPtr) (free_page->next)) < ((COMiC_UIntPtr) page))
        { free_page = free_page->next; }
    }


    if (COMiC_FALSE)
    {
        resolve:
        page->next = free_page->next;
        free_page->next = page;
    }
    page->next = NULL;
    page->is_free = COMiC_TRUE;
    return COMiC_SUCCESS;


    for (; free_page != NULL && free_page->owner != page->owner; free_page = free_page->next)
    {}


    if (free_page == NULL)
    {
        return COMiC_Error_Set(
                error,
                COMiC_ErrNo_FreeInvalidPointer,
                "Pointer doesn't correspond to specified chunk extra buffer (can't locate owner)",
                NULL,
                COMiC_Error_NoDealloc
        );
    }

    for (; free_page->next != NULL && ((COMiC_UIntPtr) (free_page->next)) < ((COMiC_UIntPtr) page); free_page = free_page->next)
    {}

    page->next = page->prev->next;


}