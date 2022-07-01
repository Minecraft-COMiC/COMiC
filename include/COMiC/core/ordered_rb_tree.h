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

static constexpr inline COMiC_IfError COMiC_OrderedRedBlackTree_Init(
        COMiC_Out COMiC_OrderedRedBlackTree *self,
        COMiC_Out COMiC_Error *error
) noexcept
{
    if (COMiC_RedBlackTree_Init((COMiC_RedBlackTree *) self, error))
    { return COMiC_ERROR; }

    self->first = NULL;
    self->last = NULL;

    return COMiC_SUCCESS;
}

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

static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_PrevSubKey(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_PrevSubKey((COMiC_RedBlackTree *) tree, (COMiC_RedBlackTree_Node *) node)); }

static constexpr inline COMiC_OrderedRedBlackTree_Node *_COMiC_OrderedRedBlackTree_NextSubKey(
        COMiC_In COMiC_OrderedRedBlackTree *tree,
        COMiC_In COMiC_OrderedRedBlackTree_Node *node
)
{ return (COMiC_OrderedRedBlackTree_Node *) (_COMiC_RedBlackTree_NextSubKey((COMiC_RedBlackTree *) tree, (COMiC_RedBlackTree_Node *) node)); }

static constexpr inline void _COMiC_OrderedRedBlackTree_RotateLeft(
        COMiC_In COMiC_OrderedRedBlackTree *self,
        COMiC_In COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_In COMiC_OrderedRedBlackTree_Node *top
) noexcept
{ _COMiC_RedBlackTree_RotateLeft((COMiC_RedBlackTree *) self, (COMiC_RedBlackTree_Node **) parent_child_p, (COMiC_RedBlackTree_Node *) top); }

static constexpr inline void _COMiC_OrderedRedBlackTree_RotateRight(
        COMiC_In COMiC_OrderedRedBlackTree *self,
        COMiC_In COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_In COMiC_OrderedRedBlackTree_Node *top
) noexcept
{ _COMiC_RedBlackTree_RotateRight((COMiC_RedBlackTree *) self, (COMiC_RedBlackTree_Node **) parent_child_p, (COMiC_RedBlackTree_Node *) top); }

static constexpr inline COMiC_IfError COMiC_OrderedRedBlackTree_Link(
        COMiC_In COMiC_OrderedRedBlackTree *self,
        COMiC_In COMiC_OrderedRedBlackTree_Node *parent,
        COMiC_In COMiC_OrderedRedBlackTree_Node **parent_child_p,
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

    return COMiC_RedBlackTree_Link(
            (COMiC_RedBlackTree *) self,
            (COMiC_RedBlackTree_Node *) parent,
            (COMiC_RedBlackTree_Node **) parent_child_p,
            (COMiC_RedBlackTree_Node *) node
    );
}

static inline COMiC_IfError COMiC_OrderedRedBlackTree_UnLink(
        COMiC_In COMiC_OrderedRedBlackTree *self,
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

    return COMiC_RedBlackTree_UnLink(
            (COMiC_RedBlackTree *) self,
            (COMiC_RedBlackTree_Node *) node
    );
}

static constexpr inline void _COMiC_OrderedRedBlackTree_LinkInsteadOf(
        COMiC_OrderedRedBlackTree *tree,
        COMiC_OrderedRedBlackTree_Node *parent,
        COMiC_OrderedRedBlackTree_Node **parent_child_p,
        COMiC_OrderedRedBlackTree_Node *node,
        COMiC_OrderedRedBlackTree_Node *destination
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

# ifdef __cplusplus
};
#endif

#endif /* COMiC_Core_ORDERED_RB_TREE_H */
