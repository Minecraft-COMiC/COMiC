#ifndef COMiC_Core_DOUBLY_LINKED_LIST_H
# define COMiC_Core_DOUBLY_LINKED_LIST_H


# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_DoublyLinkedList_Node
{
    struct COMiC_DoublyLinkedList_Node *prev;
    struct COMiC_DoublyLinkedList_Node *next;
} COMiC_DoublyLinkedList_Node;

typedef struct COMiC_DoublyLinkedList
{
    COMiC_DoublyLinkedList_Node *first;
    COMiC_DoublyLinkedList_Node *last;
} COMiC_DoublyLinkedList;


static constexpr inline void COMiC_DoublyLinkedList_Constructor(
        COMiC_Out COMiC_DoublyLinkedList *self
) noexcept
{
    self->first = NULL;
    self->last = NULL;
}

static constexpr inline void COMiC_DoublyLinkedList_Destructor(
        COMiC_In COMiC_DoublyLinkedList *self
) noexcept
{}

static constexpr inline COMiC_Bool COMiC_DoublyLinkedList_IsEmpty(
        COMiC_In COMiC_DoublyLinkedList *self
) noexcept
{ return self->first == NULL; }

static constexpr inline COMiC_DoublyLinkedList_Node *COMiC_DoublyLinkedList_GetFirst(
        COMiC_In COMiC_DoublyLinkedList *self
) noexcept
{ return self->first; }

static constexpr inline COMiC_DoublyLinkedList_Node *COMiC_DoublyLinkedList_GetLast(
        COMiC_In COMiC_DoublyLinkedList *self
) noexcept
{ return self->last; }

static constexpr inline COMiC_DoublyLinkedList_Node *COMiC_DoublyLinkedList_Node_GetPrev(
        COMiC_In COMiC_DoublyLinkedList_Node *self
) noexcept
{ return self->prev; }

static constexpr inline COMiC_DoublyLinkedList_Node *COMiC_DoublyLinkedList_Node_GetNext(
        COMiC_In COMiC_DoublyLinkedList_Node *self
) noexcept
{ return self->next; }

static constexpr inline void COMiC_DoublyLinkedList_Link(
        COMiC_InOut COMiC_DoublyLinkedList *self,
        COMiC_InOut COMiC_DoublyLinkedList_Node *node
) noexcept
{
    if (self->last == NULL)
    {
        self->first = self->last = node;
        node->prev = node->next = NULL;
    }
    else
    {
        self->last->next = node;
        node->prev = self->last;
        node->next = NULL;
        self->last = node;
    }
}

static constexpr inline void COMiC_DoublyLinkedList_UnLink(
        COMiC_InOut COMiC_DoublyLinkedList *self,
        COMiC_InOut COMiC_DoublyLinkedList_Node *node
) noexcept
{
    if (node->prev != NULL)
    { node->prev->next = node->next; }
    else
    { self->first = node->next; }

    if (node->next != NULL)
    { node->next->prev = node->prev; }
    else
    { self->last = node->prev; }
}

typedef struct COMiC_DoublyLinkedList_Iterator
{
    COMiC_DoublyLinkedList_Node *pos;
} COMiC_DoublyLinkedList_Iterator;


static constexpr inline void COMiC_DoublyLinkedList_Iterator_Constructor(
        COMiC_Out COMiC_DoublyLinkedList_Iterator *self,
        COMiC_In COMiC_DoublyLinkedList *list
)
{
    self->pos = list->first;
}

static constexpr inline void COMiC_DoublyLinkedList_Iterator_Destructor(
        COMiC_In COMiC_DoublyLinkedList_Iterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_DoublyLinkedList_Iterator_Next(
        COMiC_InOut COMiC_DoublyLinkedList_Iterator *self,
        COMiC_Out COMiC_DoublyLinkedList_Node **pos
)
{
    *pos = self->pos;

    if (self->pos != NULL)
    { self->pos = self->pos->next; }
}

typedef struct COMiC_DoublyLinkedList_ReversedIterator
{
    COMiC_DoublyLinkedList_Node *pos;
} COMiC_DoublyLinkedList_ReversedIterator;


static constexpr inline void COMiC_DoublyLinkedList_ReversedIterator_Constructor(
        COMiC_Out COMiC_DoublyLinkedList_ReversedIterator *self,
        COMiC_In COMiC_DoublyLinkedList *list
)
{
    self->pos = list->last;
}

static constexpr inline void COMiC_DoublyLinkedList_ReversedIterator_Destructor(
        COMiC_In COMiC_DoublyLinkedList_ReversedIterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_DoublyLinkedList_ReversedIterator_Next(
        COMiC_InOut COMiC_DoublyLinkedList_ReversedIterator *self,
        COMiC_Out COMiC_DoublyLinkedList_Node **pos
)
{
    *pos = self->pos;

    if (self->pos != NULL)
    { self->pos = self->pos->prev; }
}

typedef struct COMiC_DoublyLinkedList_FastFreeIterator
{
    COMiC_DoublyLinkedList_Iterator iterator;
} COMiC_DoublyLinkedList_FastFreeIterator;


static constexpr inline void COMiC_DoublyLinkedList_FastFreeIterator_Constructor(
        COMiC_Out COMiC_DoublyLinkedList_FastFreeIterator *self,
        COMiC_In COMiC_DoublyLinkedList *tree
)
{ COMiC_DoublyLinkedList_Iterator_Constructor(&(self->iterator), tree); }

static constexpr inline void COMiC_DoublyLinkedList_FastFreeIterator_Destructor(
        COMiC_In COMiC_DoublyLinkedList_FastFreeIterator *self
)
{ COMiC_DoublyLinkedList_Iterator_Destructor(&(self->iterator)); }

static constexpr inline void COMiC_DoublyLinkedList_FastFreeIterator_Next(
        COMiC_InOut COMiC_DoublyLinkedList_FastFreeIterator *self,
        COMiC_Out COMiC_DoublyLinkedList_Node **node
)
{ COMiC_DoublyLinkedList_Iterator_Next(&(self->iterator), node); }

static constexpr inline void _COMiC_DoublyLinkedList_LinkInsteadOf(
        COMiC_InOut COMiC_DoublyLinkedList *list,
        COMiC_Out COMiC_DoublyLinkedList_Node *node,
        COMiC_In COMiC_DoublyLinkedList_Node *destination
)
{
    node->prev = destination->prev;
    if (destination->prev == NULL)
    { list->first = node; }
    else
    { node->prev->next = node; }
    node->next = destination->next;
    if (destination->next == NULL)
    { list->last = node; }
    else
    { node->next->prev = node; }
}

# ifdef __cplusplus
}
# endif

#endif /* COMiC_Core_DOUBLY_LINKED_LIST_H */
