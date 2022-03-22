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
    if (size + sizeof(struct COMiC_ChunkExtraBuffer_Page_Meta) >= sizeof(union _COMiC_ChunkExtraBuffer_BigPage))
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
    struct COMiC_ChunkExtraBuffer_Buffer_Meta *meta = page->owner->owner;

    if (meta != &(self->small_pages) && meta != &(self->big_pages))
    {
        return COMiC_Error_Set(
                error,
                COMiC_ErrNo_FreeInvalidPointer,
                "Pointer doesn't correspond to specified chunk extra buffer",
                NULL,
                COMiC_Error_NoDealloc
        );
    }


}