#ifndef COMiC_Core_ORDERED_RB_TREE_H
# define COMiC_Core_ORDERED_RB_TREE_H

# include "types.h"
# include "error.h"
# include "rb_tree.h"

# ifdef __cplusplus
extern "C" {
#endif

#define _COMiC_OrderedRedBlackTree_Color _COMiC_RedBlackTree_Color

typedef struct COMiC_OrderedRedBlackTree_Node
{
    struct COMiC_RedBlackTree_Node node;
    struct COMiC_OrderedRedBlackTree_Node *prev;
    struct COMiC_OrderedRedBlackTree_Node *next;
} COMiC_OrderedRedBlackTree_Node;

typedef struct COMiC_OrderedRedBlackTree
{
    COMiC_RedBlackTree tree;
    COMiC_OrderedRedBlackTree_Node *first;
    COMiC_OrderedRedBlackTree_Node *last;
} COMiC_OrderedRedBlackTree;

COMiC_Constructor
static constexpr inline void COMiC_OrderedRedBlackTree_Init(
        COMiC_Out COMiC_OrderedRedBlackTree *self
) noexcept
{
    COMiC_RedBlackTree_Init(&(self->tree));
    self->first = NULL;
    self->last = NULL;
}

COMiC_Destructor
static constexpr inline void COMiC_OrderedRedBlackTree_Finalize(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{ COMiC_RedBlackTree_Finalize((COMiC_RedBlackTree *) self); }

static constexpr inline COMiC_Bool COMiC_OrderedRedBlackTree_IsEmpty(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{ return COMiC_RedBlackTree_IsEmpty((COMiC_RedBlackTree *) self); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_GetRoot(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (COMiC_RedBlackTree_GetRoot((COMiC_RedBlackTree *) self)); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_GetAny(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (COMiC_RedBlackTree_GetAny((COMiC_RedBlackTree *) self)); }


static constexpr inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetParent(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (COMiC_RedBlackTree_Node_GetParent((COMiC_RedBlackTree_Node *) self, owner)); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetLeft(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (COMiC_RedBlackTree_Node_GetLeft((COMiC_RedBlackTree_Node *) self, owner)); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetRight(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (COMiC_RedBlackTree_Node_GetRight((COMiC_RedBlackTree_Node *) self, owner)); }

static constexpr inline enum _COMiC_OrderedRedBlackTree_Color COMiC_OrderedRedBlackTree_Node_GetColor(
        COMiC_In COMiC_RedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return (enum _COMiC_OrderedRedBlackTree_Color) (COMiC_RedBlackTree_Node_GetColor((COMiC_RedBlackTree_Node *) self, owner)); }


static constexpr inline COMiC_OrderedRedBlackTree_Node **_COMiC_OrderedRedBlackTree_CalcParentChildP(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node **) (_COMiC_RedBlackTree_CalcParentChildP((COMiC_RedBlackTree *) tree, (COMiC_RedBlackTree_Node *) node)); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_MaxSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_MaxSubNode((COMiC_RedBlackTree *) tree, (COMiC_RedBlackTree_Node *) node)); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_MinSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_MinSubNode((COMiC_RedBlackTree *) tree, (COMiC_RedBlackTree_Node *) node)); }

static constexpr inline void _COMiC_OrderedRedBlackTree_RotateLeft(
        COMiC_InOut COMiC_OrderedRedBlackTree *self,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *top
) noexcept
{ _COMiC_RedBlackTree_RotateLeft((COMiC_RedBlackTree *) self, (COMiC_RedBlackTree_Node **) parent_child_p, (COMiC_RedBlackTree_Node *) top); }

static constexpr inline void _COMiC_OrderedRedBlackTree_RotateRight(
        COMiC_InOut COMiC_OrderedRedBlackTree *self,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *top
) noexcept
{ _COMiC_RedBlackTree_RotateRight((COMiC_RedBlackTree *) self, (COMiC_RedBlackTree_Node **) parent_child_p, (COMiC_RedBlackTree_Node *) top); }

static constexpr inline void COMiC_OrderedRedBlackTree_Link(
        COMiC_InOut COMiC_OrderedRedBlackTree *self,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *parent,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_Out COMiC_OrderedRedBlackTree_Node *node
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

    COMiC_RedBlackTree_Link(
            (COMiC_RedBlackTree *) self,
            (COMiC_RedBlackTree_Node *) parent,
            (COMiC_RedBlackTree_Node **) parent_child_p,
            (COMiC_RedBlackTree_Node *) node
    );
}

static inline void COMiC_OrderedRedBlackTree_UnLink(
        COMiC_InOut COMiC_OrderedRedBlackTree *self,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
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

    COMiC_RedBlackTree_UnLink(
            (COMiC_RedBlackTree *) self,
            (COMiC_RedBlackTree_Node *) node
    );
}

static constexpr inline void _COMiC_OrderedRedBlackTree_LinkInsteadOf(
        COMiC_InOut COMiC_OrderedRedBlackTree *tree,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *parent,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *node,
        COMiC_In COMiC_OrderedRedBlackTree_Node *destination
)
{
    _COMiC_RedBlackTree_LinkInsteadOf((COMiC_RedBlackTree *) tree, (COMiC_RedBlackTree_Node *) parent, (COMiC_RedBlackTree_Node **) parent_child_p, (COMiC_RedBlackTree_Node *) node, (COMiC_RedBlackTree_Node *) destination);
    node->prev = destination->prev;
    if (destination->prev == NULL)
    { tree->first = node; }
    else
    { node->prev->next = node; }
    node->next = destination->next;
    if (destination->next == NULL)
    { tree->last = node; }
    else
    { node->next->prev = node; }
}

typedef struct COMiC_OrderedRedBlackTree_SortedIterator
{
    COMiC_RedBlackTree_SortedIterator sorted_iterator;
} COMiC_OrderedRedBlackTree_SortedIterator;

COMiC_Constructor
static constexpr inline void COMiC_OrderedRedBlackTree_SortedIterator_Init(
        COMiC_Out COMiC_OrderedRedBlackTree_SortedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_RedBlackTree_SortedIterator_Init(&(self->sorted_iterator), &(tree->tree)); }

COMiC_Destructor
static constexpr inline void COMiC_OrderedRedBlackTree_SortedIterator_Finalize(
        COMiC_In COMiC_OrderedRedBlackTree_SortedIterator *self
)
{ return COMiC_RedBlackTree_SortedIterator_Finalize(&(self->sorted_iterator)); }


static constexpr inline void COMiC_OrderedRedBlackTree_SortedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_SortedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{ COMiC_RedBlackTree_SortedIterator_Next(&(self->sorted_iterator), (COMiC_RedBlackTree_Node **) pos); }

typedef struct COMiC_OrderedRedBlackTree_ReversedSortedIterator
{
    COMiC_RedBlackTree_ReversedSortedIterator reversed_sorted_iterator;
} COMiC_OrderedRedBlackTree_ReversedSortedIterator;

COMiC_Constructor
static constexpr inline void COMiC_OrderedRedBlackTree_ReversedSortedIterator_Init(
        COMiC_Out COMiC_OrderedRedBlackTree_ReversedSortedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_RedBlackTree_ReversedSortedIterator_Init(&(self->reversed_sorted_iterator), &(tree->tree)); }

COMiC_Destructor
static constexpr inline void COMiC_OrderedRedBlackTree_ReversedSortedIterator_Finalize(
        COMiC_In COMiC_OrderedRedBlackTree_ReversedSortedIterator *self
)
{ COMiC_RedBlackTree_ReversedSortedIterator_Finalize(&(self->reversed_sorted_iterator)); }


static constexpr inline void COMiC_OrderedRedBlackTree_ReversedSortedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_ReversedSortedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{ COMiC_RedBlackTree_ReversedSortedIterator_Next(&(self->reversed_sorted_iterator), (COMiC_RedBlackTree_Node **) pos); }


static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_DeepestLeftSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_DeepestLeftSubNode(&(tree->tree), &(node->node))); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_DeepestRightSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_DeepestRightSubNode(&(tree->tree), &(node->node))); }

typedef struct COMiC_OrderedRedBlackTree_OrderedIterator
{
    COMiC_OrderedRedBlackTree_Node *pos;
} COMiC_OrderedRedBlackTree_OrderedIterator;


COMiC_Constructor
static constexpr inline void COMiC_OrderedRedBlackTree_OrderedIterator_Init(
        COMiC_Out COMiC_OrderedRedBlackTree_OrderedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{
    self->pos = tree->first;
}

COMiC_Destructor
static constexpr inline void COMiC_OrderedRedBlackTree_OrderedIterator_Finalize(
        COMiC_In COMiC_OrderedRedBlackTree_OrderedIterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_OrderedRedBlackTree_OrderedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_OrderedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{
    *pos = self->pos;

    if (self->pos != NULL)
    { self->pos = self->pos->next; }
}

typedef struct COMiC_OrderedRedBlackTree_ReversedOrderedIterator
{
    COMiC_OrderedRedBlackTree_Node *pos;
} COMiC_OrderedRedBlackTree_ReversedOrderedIterator;


COMiC_Constructor
static constexpr inline void COMiC_OrderedRedBlackTree_ReversedOrderedIterator_Init(
        COMiC_Out COMiC_OrderedRedBlackTree_ReversedOrderedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{
    self->pos = tree->last;
}

COMiC_Destructor
static constexpr inline void COMiC_OrderedRedBlackTree_ReversedOrderedIterator_Finalize(
        COMiC_In COMiC_OrderedRedBlackTree_ReversedOrderedIterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_OrderedRedBlackTree_ReversedOrderedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_ReversedOrderedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{
    *pos = self->pos;

    if (self->pos != NULL)
    { self->pos = self->pos->prev; }
}

typedef struct COMiC_OrderedRedBlackTree_FastFreeIterator
{
    COMiC_OrderedRedBlackTree_OrderedIterator ordered_iterator;
} COMiC_OrderedRedBlackTree_FastFreeIterator;


COMiC_Constructor
static constexpr inline void COMiC_OrderedRedBlackTree_FastFreeIterator_Init(
        COMiC_Out COMiC_OrderedRedBlackTree_FastFreeIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_OrderedRedBlackTree_OrderedIterator_Init(&(self->ordered_iterator), tree); }

COMiC_Destructor
static constexpr inline void COMiC_OrderedRedBlackTree_FastFreeIterator_Finalize(
        COMiC_In COMiC_OrderedRedBlackTree_FastFreeIterator *self
)
{ COMiC_OrderedRedBlackTree_OrderedIterator_Finalize(&(self->ordered_iterator)); }


static constexpr inline void COMiC_OrderedRedBlackTree_FastFreeIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_FastFreeIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **node
)
{ COMiC_OrderedRedBlackTree_OrderedIterator_Next(&(self->ordered_iterator), node); }

# ifdef __cplusplus
}
#endif

#endif /* COMiC_Core_ORDERED_RB_TREE_H */
