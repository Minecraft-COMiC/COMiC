#include <string.h>

#include <COMiC.h>

struct COMiC_SingleArena_Head
{
    struct COMiC_SingleArena_Head *next;
    COMiC_USize capacity;
    COMiC_USize used;
};

struct COMiC_SingleArena_PageMeta
{
    struct COMiC_SingleArena_PageMeta *next;
    struct COMiC_SingleArena_Head *owner;
};


COMiC_Constructor
COMiC_IfError COMiC_SingleArena_Init(
        COMiC_Out struct COMiC_SingleArena *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Heap *heap,
        COMiC_In COMiC_USize page_size
)
{
    *(COMiC_Heap **) &(self->heap) = heap;
    self->capacity = 0;
    self->free = 0;
    *(COMiC_USize *) &(self->page_size) = page_size;
    self->buffer = NULL;
    self->top = NULL;
    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_SingleArena_Finalize(
        COMiC_In struct COMiC_SingleArena *self,
        COMiC_Out COMiC_Error *error
)
{
    struct COMiC_SingleArena_Head *p = self->buffer;
    struct COMiC_SingleArena_Head *next;
    while (p != NULL)
    {
        next = p->next;
        if (COMiC_Free(
                self->heap,
                error,
                (void **) &p
        ))
        {
            self->buffer = p;
            return COMiC_ERROR;
        }
        p = next;
    }
    return COMiC_SUCCESS;
}


COMiC_IfError COMiC_SingleArena_Push(
        COMiC_In struct COMiC_SingleArena *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void *data,
        COMiC_In COMiC_USize data_size,
        COMiC_Out void **stored
)
{
    register struct COMiC_SingleArena_PageMeta *page;

    if (data_size > self->page_size)
    {
        return COMiC_Error_Set(
                error,
                COMiC_ErrNo_Overflow,
                "So big element for single-page buffer",
                NULL,
                COMiC_Error_NoDealloc
        );
    }

    if (self->top == NULL)
    {
        struct COMiC_SingleArena_Head *new_buffer;
        COMiC_USize new_buffer_size = self->capacity;
        const register COMiC_USize final_page_size = sizeof(struct COMiC_SingleArena_PageMeta) + self->page_size;
        if (new_buffer_size == 0)
        { new_buffer_size = 256; }

        if (COMiC_Alloc(
                self->heap,
                error,
                "Can't extend single buffer (may be not enough memory)",
                (void **) &new_buffer,
                sizeof(struct COMiC_SingleArena_Head) + final_page_size * new_buffer_size
        ))
        {
            return COMiC_ERROR;
        }

        new_buffer->next = self->buffer;
        new_buffer->capacity = new_buffer_size;
        new_buffer->used = 0;
        self->buffer = new_buffer;
        self->capacity += new_buffer_size;
        self->free += new_buffer_size;

        self->top = page = (struct COMiC_SingleArena_PageMeta *) (new_buffer + 1);
        do
        {
            page->next = (struct COMiC_SingleArena_PageMeta *) (((COMiC_UIntPtr) page) + final_page_size);
            page->owner = new_buffer;
            page = page->next;
        } while (--new_buffer_size > 0);
        ((struct COMiC_SingleArena_PageMeta *) (((COMiC_UIntPtr) page) - final_page_size))->next = NULL;
    }

    page = self->top;
    self->top = page->next;
    self->free--;
    page->owner->used++;
    *stored = ++page;
    memcpy(*stored, data, data_size);

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_SingleArena_Pop(
        COMiC_In struct COMiC_SingleArena *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **stored
)
{
    struct COMiC_SingleArena_PageMeta *page;
    page = (struct COMiC_SingleArena_PageMeta *) (*stored) - 1;
    page->next = self->top;
    self->top = page;
    page->owner->used--;
    self->free++;
    return COMiC_SUCCESS;
}


COMiC_IfError COMiC_SingleArena_Clear(
        COMiC_In struct COMiC_SingleArena *self,
        COMiC_Out COMiC_Error *error
)
{
    struct COMiC_SingleArena_Head *p = self->buffer;
    struct COMiC_SingleArena_Head **prev = (struct COMiC_SingleArena_Head **) &(self->buffer);
    struct COMiC_SingleArena_Head *next;

    while (p != NULL)
    {
        next = p->next;
        if (p->used > 0)
        {
            prev = &(p->next);
            p = next;
            continue;
        }

        *prev = next;
        self->capacity -= p->capacity;
        if (COMiC_Free(
                self->heap,
                error,
                (void **) &p
        ))
        {
            return COMiC_ERROR;
        }
        p = *prev;
    }
    return COMiC_SUCCESS;
}