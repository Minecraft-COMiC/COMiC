#ifndef COMiC_Core_LINKED_MEMORY_H
# define COMiC_Core_LINKED_MEMORY_H

# include "types.h"
# include "error.h"
# include "doubly_linked_list.h"
# include "os.h"

# ifdef __cplusplus
extern "C" {
# endif

struct COMiC_LinkedMemory_Node
{
    COMiC_DoublyLinkedList_Node list_node;
};

typedef struct COMiC_LinkedMemory
{
    COMiC_DoublyLinkedList list;
} COMiC_LinkedMemory;

static constexpr inline void COMiC_LinkedMemory_Constructor(
        COMiC_Out COMiC_LinkedMemory *self
) noexcept
{
    COMiC_DoublyLinkedList_Constructor(&(self->list));
}

static constexpr inline COMiC_Bool COMiC_LinkedMemory_IsEmpty(
        COMiC_In COMiC_LinkedMemory *self
) noexcept
{ return COMiC_DoublyLinkedList_IsEmpty(&(self->list)); }

static inline COMiC_IfError COMiC_LinkedMemory_Alloc(
        COMiC_InOut COMiC_LinkedMemory *self,
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
) noexcept
{
    if (COMiC_NativeAllocOnPages(error, pointer, size))
    { return COMiC_ERROR; }

    COMiC_DoublyLinkedList_Link(&(self->list), &(((struct COMiC_LinkedMemory_Node *) (*pointer))->list_node));
    *pointer = (void *) (((COMiC_UIntPtr) (*pointer)) + sizeof(struct COMiC_LinkedMemory_Node));

    return COMiC_SUCCESS;
}

static inline COMiC_IfError COMiC_LinkedMemory_DeAlloc(
        COMiC_InOut COMiC_LinkedMemory *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void *pointer
) noexcept
{
    pointer = (void *) (((COMiC_UIntPtr) pointer) - sizeof(struct COMiC_LinkedMemory_Node));
    COMiC_DoublyLinkedList_UnLink(&(self->list), &(((struct COMiC_LinkedMemory_Node *) pointer)->list_node));

    if (COMiC_NativeDeAllocOnPages(error, pointer))
    { return COMiC_ERROR; }

    return COMiC_SUCCESS;
}


typedef struct COMiC_LinkedMemory_FastFreeIterator
{
    COMiC_DoublyLinkedList_FastFreeIterator list_iterator;
} COMiC_LinkedMemory_FastFreeIterator;

static constexpr inline void COMiC_LinkedMemory_FastFreeIterator_Constructor(
        COMiC_Out COMiC_LinkedMemory_FastFreeIterator *self,
        COMiC_In COMiC_LinkedMemory *lm
)
{ COMiC_DoublyLinkedList_FastFreeIterator_Constructor(&(self->list_iterator), &(lm->list)); }


static constexpr inline void COMiC_LinkedMemory_FastFreeIterator_Destructor(
        COMiC_In COMiC_LinkedMemory_FastFreeIterator *self
)
{ COMiC_DoublyLinkedList_FastFreeIterator_Destructor(&(self->list_iterator)); }


static inline void COMiC_LinkedMemory_FastFreeIterator_Next(
        COMiC_InOut COMiC_LinkedMemory_FastFreeIterator *self,
        COMiC_Out void **pos
)
{
    COMiC_DoublyLinkedList_FastFreeIterator_Next(&(self->list_iterator), (COMiC_DoublyLinkedList_Node **) pos);
    if (*pos != NULL)
    { *pos = (void *) (((COMiC_UIntPtr) pos) - offsetof(struct COMiC_LinkedMemory_Node, list_node) + sizeof(struct COMiC_LinkedMemory_Node)); }
}

static inline COMiC_IfError COMiC_LinkedMemory_Destructor(
        COMiC_Out COMiC_LinkedMemory *self,
        COMiC_Out COMiC_Error *error
) noexcept
{
    COMiC_LinkedMemory_FastFreeIterator iterator;
    void *pos;
    COMiC_LinkedMemory_FastFreeIterator_Constructor(&iterator, self);
    while (COMiC_TRUE)
    {
        COMiC_LinkedMemory_FastFreeIterator_Next(&iterator, &pos);
        if (pos == NULL)
        { break; }
        if (COMiC_LinkedMemory_DeAlloc(self, error, pos))
        {
            COMiC_LinkedMemory_FastFreeIterator_Destructor(&iterator);
            return COMiC_ERROR;
        }
    }
    COMiC_LinkedMemory_FastFreeIterator_Destructor(&iterator);

    return COMiC_SUCCESS;
}

# ifdef __cplusplus
}
# endif

#endif /* COMiC_Core_LINKED_MEMORY_H */
