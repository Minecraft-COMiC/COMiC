#ifndef COMiC_Core_ORDERED_RB_TREE_H
# define COMiC_Core_ORDERED_RB_TREE_H

# include "types.h"
# include "error.h"
# include "rb_tree.h"
# include "doubly_linked_list.h"

# ifdef __cplusplus
extern "C" {
#endif

#define _COMiC_OrderedRedBlackTree_Color _COMiC_RedBlackTree_Color

typedef struct COMiC_OrderedRedBlackTree_Node
{
    COMiC_RedBlackTree_Node tree_node;
    COMiC_DoublyLinkedList_Node list_node;
} COMiC_OrderedRedBlackTree_Node;

typedef struct COMiC_OrderedRedBlackTree
{
    COMiC_RedBlackTree tree;
    COMiC_DoublyLinkedList list;
} COMiC_OrderedRedBlackTree;

static constexpr inline void COMiC_OrderedRedBlackTree_Constructor(
        COMiC_Out COMiC_OrderedRedBlackTree *self
) noexcept
{
    COMiC_RedBlackTree_Constructor(&(self->tree));
    COMiC_DoublyLinkedList_Constructor(&(self->list));
}

static constexpr inline void COMiC_OrderedRedBlackTree_Destructor(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{
    COMiC_RedBlackTree_Destructor(&(self->tree));
    COMiC_DoublyLinkedList_Destructor(&(self->list));
}

static constexpr inline COMiC_Bool COMiC_OrderedRedBlackTree_IsEmpty(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{ return COMiC_RedBlackTree_IsEmpty(&(self->tree)); }

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_GetRoot(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_RedBlackTree_GetRoot(&(self->tree)))) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node)); }

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_GetFirst(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_DoublyLinkedList_GetFirst(&(self->list)))) - offsetof(COMiC_OrderedRedBlackTree_Node, list_node)); }

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_GetLast(
        COMiC_In COMiC_OrderedRedBlackTree *self
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_DoublyLinkedList_GetLast(&(self->list)))) - offsetof(COMiC_OrderedRedBlackTree_Node, list_node)); }

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetParent(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_OrderedRedBlackTree *owner
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_RedBlackTree_Node_GetParent(&(self->tree_node), &(owner->tree)))) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node)); }

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetLeft(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_OrderedRedBlackTree *owner
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_RedBlackTree_Node_GetLeft(&(self->tree_node), &(owner->tree)))) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node)); }

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetRight(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_OrderedRedBlackTree *owner
) noexcept
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_RedBlackTree_Node_GetRight(&(self->tree_node), &(owner->tree)))) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node)); }

static constexpr inline enum _COMiC_OrderedRedBlackTree_Color COMiC_OrderedRedBlackTree_Node_GetColor(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_OrderedRedBlackTree *owner
) noexcept
{ return (enum _COMiC_OrderedRedBlackTree_Color) (COMiC_RedBlackTree_Node_GetColor(&(self->tree_node), &(owner->tree))); }


static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetPrev(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_OrderedRedBlackTree *owner
) noexcept
{
    COMiC_UnusedArg(owner);
    return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_DoublyLinkedList_Node_GetPrev(&(self->list_node)))) - offsetof(COMiC_OrderedRedBlackTree_Node, list_node));
}

static inline COMiC_OrderedRedBlackTree_Node *COMiC_OrderedRedBlackTree_Node_GetNext(
        COMiC_In COMiC_OrderedRedBlackTree_Node *self,
        COMiC_In COMiC_OrderedRedBlackTree *owner
) noexcept
{
    COMiC_UnusedArg(owner);
    return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (COMiC_DoublyLinkedList_Node_GetNext(&(self->list_node)))) - offsetof(COMiC_OrderedRedBlackTree_Node, list_node));
}

static inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_MaxSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (_COMiC_RedBlackTree_MaxSubNode(&(tree->tree), &(node->tree_node)))) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node)); }

static inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_MinSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) (_COMiC_RedBlackTree_MinSubNode(&(tree->tree), &(node->tree_node)))) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node)); }

static constexpr inline void COMiC_OrderedRedBlackTree_Link(
        COMiC_InOut COMiC_OrderedRedBlackTree *self,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *parent,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_Out COMiC_OrderedRedBlackTree_Node *node
) noexcept
{
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
    COMiC_RedBlackTree_UnLink(
            &(self->tree),
            &(node->tree_node)
    );
    COMiC_DoublyLinkedList_UnLink(
            &(self->list),
            &(node->list_node)
    );
}

static constexpr inline void _COMiC_OrderedRedBlackTree_LinkInsteadOf(
        COMiC_InOut COMiC_OrderedRedBlackTree *tree,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node *parent,
        COMiC_InOut COMiC_RedBlackTree_Node **parent_child_p,
        COMiC_Out COMiC_OrderedRedBlackTree_Node *node,
        COMiC_In COMiC_OrderedRedBlackTree_Node *destination
)
{
    _COMiC_RedBlackTree_LinkInsteadOf(&(tree->tree), &(parent->tree_node),  parent_child_p, &(node->tree_node), &(destination->tree_node));
    _COMiC_DoublyLinkedList_LinkInsteadOf(&(tree->list), &(node->list_node), &(destination->list_node));
}

typedef struct COMiC_OrderedRedBlackTree_SortedIterator
{
    COMiC_RedBlackTree_SortedIterator sorted_iterator;
} COMiC_OrderedRedBlackTree_SortedIterator;

static constexpr inline void COMiC_OrderedRedBlackTree_SortedIterator_Constructor(
        COMiC_Out COMiC_OrderedRedBlackTree_SortedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_RedBlackTree_SortedIterator_Constructor(&(self->sorted_iterator), &(tree->tree)); }

static constexpr inline void COMiC_OrderedRedBlackTree_SortedIterator_Destructor(
        COMiC_In COMiC_OrderedRedBlackTree_SortedIterator *self
)
{ COMiC_RedBlackTree_SortedIterator_Destructor(&(self->sorted_iterator)); }


static inline void COMiC_OrderedRedBlackTree_SortedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_SortedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{
    COMiC_RedBlackTree_SortedIterator_Next(&(self->sorted_iterator), (COMiC_RedBlackTree_Node **) pos);
    *pos = (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) pos) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node));
}

typedef struct COMiC_OrderedRedBlackTree_ReversedSortedIterator
{
    COMiC_RedBlackTree_ReversedSortedIterator reversed_sorted_iterator;
} COMiC_OrderedRedBlackTree_ReversedSortedIterator;

static constexpr inline void COMiC_OrderedRedBlackTree_ReversedSortedIterator_Constructor(
        COMiC_Out COMiC_OrderedRedBlackTree_ReversedSortedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_RedBlackTree_ReversedSortedIterator_Constructor(&(self->reversed_sorted_iterator), &(tree->tree)); }

static constexpr inline void COMiC_OrderedRedBlackTree_ReversedSortedIterator_Destructor(
        COMiC_In COMiC_OrderedRedBlackTree_ReversedSortedIterator *self
)
{ COMiC_RedBlackTree_ReversedSortedIterator_Destructor(&(self->reversed_sorted_iterator)); }


static inline void COMiC_OrderedRedBlackTree_ReversedSortedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_ReversedSortedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{
    COMiC_RedBlackTree_ReversedSortedIterator_Next(&(self->reversed_sorted_iterator), (COMiC_RedBlackTree_Node **) pos);
    *pos = (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) pos) - offsetof(COMiC_OrderedRedBlackTree_Node, tree_node));
}


static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_DeepestLeftSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_DeepestLeftSubNode(&(tree->tree), &(node->tree_node))); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_DeepestRightSubNode(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_DeepestRightSubNode(&(tree->tree), &(node->tree_node))); }

typedef struct COMiC_OrderedRedBlackTree_OrderedIterator
{
    COMiC_DoublyLinkedList_Iterator list_iterator;
} COMiC_OrderedRedBlackTree_OrderedIterator;


static constexpr inline void COMiC_OrderedRedBlackTree_OrderedIterator_Constructor(
        COMiC_Out COMiC_OrderedRedBlackTree_OrderedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_DoublyLinkedList_Iterator_Constructor(&(self->list_iterator), &(tree->list)); }


static constexpr inline void COMiC_OrderedRedBlackTree_OrderedIterator_Destructor(
        COMiC_In COMiC_OrderedRedBlackTree_OrderedIterator *self
)
{ COMiC_DoublyLinkedList_Iterator_Destructor(&(self->list_iterator)); }


static inline void COMiC_OrderedRedBlackTree_OrderedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_OrderedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{
    COMiC_DoublyLinkedList_Iterator_Next(&(self->list_iterator), (COMiC_DoublyLinkedList_Node **) pos);
    *pos = (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) pos) - offsetof(COMiC_OrderedRedBlackTree_Node, list_node));
}

typedef struct COMiC_OrderedRedBlackTree_ReversedOrderedIterator
{
    COMiC_DoublyLinkedList_ReversedIterator list_iterator;
} COMiC_OrderedRedBlackTree_ReversedOrderedIterator;


static constexpr inline void COMiC_OrderedRedBlackTree_ReversedOrderedIterator_Constructor(
        COMiC_Out COMiC_OrderedRedBlackTree_ReversedOrderedIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_DoublyLinkedList_ReversedIterator_Constructor(&(self->list_iterator), &(tree->list)); }


static constexpr inline void COMiC_OrderedRedBlackTree_ReversedOrderedIterator_Destructor(
        COMiC_In COMiC_OrderedRedBlackTree_ReversedOrderedIterator *self
)
{ COMiC_DoublyLinkedList_ReversedIterator_Destructor(&(self->list_iterator)); }


static inline void COMiC_OrderedRedBlackTree_ReversedOrderedIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_ReversedOrderedIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **pos
)
{
    COMiC_DoublyLinkedList_ReversedIterator_Next(&(self->list_iterator), (COMiC_DoublyLinkedList_Node **) pos);
    *pos = (COMiC_OrderedRedBlackTree_Node *) (((COMiC_UIntPtr) pos) - offsetof(COMiC_OrderedRedBlackTree_Node, list_node));
}


typedef struct COMiC_OrderedRedBlackTree_FastFreeIterator
{
    COMiC_OrderedRedBlackTree_OrderedIterator ordered_iterator;
} COMiC_OrderedRedBlackTree_FastFreeIterator;


static constexpr inline void COMiC_OrderedRedBlackTree_FastFreeIterator_Constructor(
        COMiC_Out COMiC_OrderedRedBlackTree_FastFreeIterator *self,
        COMiC_In COMiC_OrderedRedBlackTree *tree
)
{ COMiC_OrderedRedBlackTree_OrderedIterator_Constructor(&(self->ordered_iterator), tree); }

static constexpr inline void COMiC_OrderedRedBlackTree_FastFreeIterator_Destructor(
        COMiC_In COMiC_OrderedRedBlackTree_FastFreeIterator *self
)
{ COMiC_OrderedRedBlackTree_OrderedIterator_Destructor(&(self->ordered_iterator)); }

static inline void COMiC_OrderedRedBlackTree_FastFreeIterator_Next(
        COMiC_InOut COMiC_OrderedRedBlackTree_FastFreeIterator *self,
        COMiC_Out COMiC_OrderedRedBlackTree_Node **node
)
{ COMiC_OrderedRedBlackTree_OrderedIterator_Next(&(self->ordered_iterator), node); }

# ifdef __cplusplus
}
#endif

#endif /* COMiC_Core_ORDERED_RB_TREE_H */
