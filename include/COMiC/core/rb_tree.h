#ifndef COMiC_Core_RB_TREE_H
# define COMiC_Core_RB_TREE_H

# include "COMiC/core/types.h"
# include "error.h"

# ifdef __cplusplus
extern "C" {
# endif

enum _COMiC_RedBlackTree_Color
{
    _COMiC_RedBlackTree_BLACK = 0,
    _COMiC_RedBlackTree_RED = 1
};

typedef struct COMiC_RedBlackTree_Node
{
    enum _COMiC_RedBlackTree_Color color;
    struct COMiC_RedBlackTree_Node *parent;
    struct COMiC_RedBlackTree_Node *left;
    struct COMiC_RedBlackTree_Node *right;
} COMiC_RedBlackTree_Node;

typedef struct COMiC_RedBlackTree
{
    COMiC_RedBlackTree_Node sentinel;
} COMiC_RedBlackTree;

COMiC_Constructor
static constexpr inline void COMiC_RedBlackTree_Init(
        COMiC_Out COMiC_RedBlackTree *self
) noexcept
{
    self->sentinel.color = _COMiC_RedBlackTree_BLACK;
    self->sentinel.parent = &(self->sentinel);
    self->sentinel.left = &(self->sentinel);
    self->sentinel.right = &(self->sentinel);
}

COMiC_Destructor
static constexpr inline void COMiC_RedBlackTree_Finalize(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{
    COMiC_UnusedArg(self);
}

static constexpr inline COMiC_Bool COMiC_RedBlackTree_IsEmpty(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{ return self->sentinel.parent == &(self->sentinel); }

static constexpr inline COMiC_RedBlackTree_Node *COMiC_RedBlackTree_GetRoot(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{ return self->sentinel.parent == &(self->sentinel) ? NULL : self->sentinel.parent; }

static constexpr inline COMiC_RedBlackTree_Node *COMiC_RedBlackTree_GetAny(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{ return COMiC_RedBlackTree_GetRoot(self); }

static constexpr inline COMiC_RedBlackTree_Node *COMiC_RedBlackTree_Node_GetParent(
        COMiC_In COMiC_RedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return self->parent == &(owner->sentinel) ? NULL : self->parent; }

static constexpr inline COMiC_RedBlackTree_Node *COMiC_RedBlackTree_Node_GetLeft(
        COMiC_In COMiC_RedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return self->left == &(owner->sentinel) ? NULL : self->left; }

static constexpr inline COMiC_RedBlackTree_Node *COMiC_RedBlackTree_Node_GetRight(
        COMiC_In COMiC_RedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{ return self->right == &(owner->sentinel) ? NULL : self->right; }

static constexpr inline enum _COMiC_RedBlackTree_Color COMiC_RedBlackTree_Node_GetColor(
        COMiC_In COMiC_RedBlackTree_Node *self,
        COMiC_In COMiC_RedBlackTree *owner
) noexcept
{
    COMiC_UnusedArg(owner);

    return self->color;
}


static constexpr inline COMiC_RedBlackTree_Node **_COMiC_RedBlackTree_CalcParentChildP(
        COMiC_In COMiC_RedBlackTree *tree,
        COMiC_In COMiC_RedBlackTree_Node *node
) noexcept
{
    if (node == node->parent->right)
    { return &(node->parent->right); }
    else if (node == node->parent->left)
    { return &(node->parent->left); }
    else
    { return &(tree->sentinel.parent); }
}

static constexpr inline COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_MaxSubNode(
        COMiC_In COMiC_RedBlackTree *tree,
        COMiC_In COMiC_RedBlackTree_Node *node
) noexcept
{
    while (node->right != &(tree->sentinel))
    { node = node->right; }
    return node;
}

static constexpr inline COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_MinSubNode(
        COMiC_In COMiC_RedBlackTree *tree,
        COMiC_In COMiC_RedBlackTree_Node *node
) noexcept
{
    while (node->left != &(tree->sentinel))
    { node = node->left; }
    return node;
}

static constexpr inline void _COMiC_RedBlackTree_RotateLeft(
        COMiC_InOut COMiC_RedBlackTree *self,
        COMiC_InOut COMiC_RedBlackTree_Node **parent_child_p,
        COMiC_InOut COMiC_RedBlackTree_Node *top
) noexcept
{
    COMiC_RedBlackTree_Node *node = top->right;
    top->right = node->left;

    if (node->left != &(self->sentinel))
    { node->left->parent = top; }

    node->parent = top->parent;

    *parent_child_p = node;

    node->left = top;
    top->parent = node;
}

static constexpr inline void _COMiC_RedBlackTree_RotateRight(
        COMiC_InOut COMiC_RedBlackTree *self,
        COMiC_InOut COMiC_RedBlackTree_Node **parent_child_p,
        COMiC_InOut COMiC_RedBlackTree_Node *top
) noexcept
{
    COMiC_RedBlackTree_Node *node = top->left;
    top->left = node->right;

    if (node->right != &(self->sentinel))
    { node->right->parent = top; }

    node->parent = top->parent;

    *parent_child_p = node;

    node->right = top;
    top->parent = node;
}

static constexpr inline void COMiC_RedBlackTree_Link(
        COMiC_InOut COMiC_RedBlackTree *self,
        COMiC_InOut COMiC_RedBlackTree_Node *parent,
        COMiC_InOut COMiC_RedBlackTree_Node **parent_child_p,
        COMiC_Out COMiC_RedBlackTree_Node *node
) noexcept
{
    node->parent = parent;
    *parent_child_p = node;
    node->left = &(self->sentinel);
    node->right = &(self->sentinel);
    if (parent == &(self->sentinel))
    {
        node->color = _COMiC_RedBlackTree_BLACK;
        return;
    }
    node->color = _COMiC_RedBlackTree_RED;

    COMiC_RedBlackTree_Node *pointer = node;
    while (pointer->parent->color == _COMiC_RedBlackTree_RED)
    {
        if (pointer->parent == pointer->parent->parent->left)
        {
            COMiC_RedBlackTree_Node *parent_sibling = pointer->parent->parent->right;
            if (parent_sibling->color == _COMiC_RedBlackTree_RED)
            {
                pointer->parent->color = _COMiC_RedBlackTree_BLACK;
                parent_sibling->color = _COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = _COMiC_RedBlackTree_RED;
                pointer = pointer->parent->parent;
            }
            else
            {
                if (pointer == pointer->parent->right)
                {
                    pointer = pointer->parent;
                    _COMiC_RedBlackTree_RotateLeft(self, _COMiC_RedBlackTree_CalcParentChildP(self, pointer), pointer);
                }

                pointer->parent->color = _COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = _COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateRight(self, _COMiC_RedBlackTree_CalcParentChildP(self, pointer->parent->parent), pointer->parent->parent);
            }
        }
        else
        {
            COMiC_RedBlackTree_Node *parent_sibling = pointer->parent->parent->left;
            if (parent_sibling->color == _COMiC_RedBlackTree_RED)
            {
                pointer->parent->color = _COMiC_RedBlackTree_BLACK;
                parent_sibling->color = _COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = _COMiC_RedBlackTree_RED;
                pointer = pointer->parent->parent;
            }
            else
            {
                if (pointer == pointer->parent->left)
                {
                    pointer = pointer->parent;
                    _COMiC_RedBlackTree_RotateLeft(self, _COMiC_RedBlackTree_CalcParentChildP(self, pointer), pointer);
                }

                pointer->parent->color = _COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = _COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateLeft(self, _COMiC_RedBlackTree_CalcParentChildP(self, pointer->parent->parent), pointer->parent->parent);
            }
        }
    }

    self->sentinel.parent->color = _COMiC_RedBlackTree_BLACK;
}

static inline void COMiC_RedBlackTree_UnLink(
        COMiC_InOut COMiC_RedBlackTree *self,
        COMiC_In COMiC_RedBlackTree_Node *node
) noexcept
{
    COMiC_RedBlackTree_Node *fix_node;
    COMiC_RedBlackTree_Node *fix_node_parent;
    COMiC_RedBlackTree_Node *pointer = node;

    if (pointer->left == &(self->sentinel))
    { fix_node = pointer->right; }
    else if (pointer->right == &(self->sentinel))
    { fix_node = pointer->left; }
    else
    {
        pointer = _COMiC_RedBlackTree_MinSubNode(self, node->right);
        fix_node = pointer->right;
        goto TWO_CHILDREN;
    }

    fix_node_parent = node->parent;
    if (fix_node != &(self->sentinel))
    { fix_node->parent = fix_node_parent; }

    if (node->parent == &(self->sentinel))
    { self->sentinel.parent = fix_node; }
    else if (fix_node_parent->left == node)
    { fix_node_parent->left = fix_node; }
    else
    { fix_node_parent->right = fix_node; }

#   if 0
    if (self->lowest == node)
    { self->lowest = (fix_node ==  &(self->sentinel)) ? fix_node_parent : _COMiC_RedBlackTree_PrevSubKey(fix_node); }

    if (self->greatest == node)
    { self->greatest = (fix_node ==  &(self->sentinel)) ? fix_node_parent : _COMiC_RedBlackTree_NextSubKey(fix_node); }
#   endif

    if (node->color == _COMiC_RedBlackTree_RED)
    { goto NOT_BLACK; }

    goto BALANCE;

    TWO_CHILDREN:
    node->left->parent = pointer;
    pointer->left = node->left;

    if (pointer == node->right)
    { fix_node_parent = pointer; }
    else
    {
        fix_node_parent = pointer->parent;
        if (fix_node != &(self->sentinel))
        { fix_node->parent = fix_node_parent; }

        fix_node_parent->left = fix_node;
        pointer->right = node->right;
        node->right->right = pointer;
    }

    if (node->parent == &(self->sentinel))
    { self->sentinel.parent = pointer; }
    else if (node->parent->left == node)
    { node->parent->left = pointer; }
    else
    { node->parent->right = pointer; }

    pointer->parent = node->parent;
    if (node->color == _COMiC_RedBlackTree_RED)
    { goto NOT_BLACK; }
    node->color = pointer->color;
    pointer->color = _COMiC_RedBlackTree_BLACK;

    BALANCE:
    while ((fix_node_parent = fix_node->parent) != &(self->sentinel) && fix_node->color == _COMiC_RedBlackTree_BLACK)
    {
        if (fix_node == fix_node_parent->left)
        {
            pointer = fix_node_parent->right;
            if (pointer->color == _COMiC_RedBlackTree_RED)
            {
                pointer->color = _COMiC_RedBlackTree_BLACK;
                fix_node_parent->color = _COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateLeft(self, _COMiC_RedBlackTree_CalcParentChildP(self, fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->right;
            }

            if (pointer == &(self->sentinel))
            { fix_node = fix_node_parent; }
            else if (pointer->left->color == _COMiC_RedBlackTree_BLACK && pointer->right->color == _COMiC_RedBlackTree_BLACK)
            {
                pointer->color = _COMiC_RedBlackTree_RED;
                fix_node = fix_node_parent;
            }
            else
            {
                if (pointer->right->color == _COMiC_RedBlackTree_BLACK)
                {
                    pointer->left->color = _COMiC_RedBlackTree_BLACK;
                    pointer->color = _COMiC_RedBlackTree_RED;
                    _COMiC_RedBlackTree_RotateRight(self, _COMiC_RedBlackTree_CalcParentChildP(self, pointer), pointer);
                    pointer = fix_node_parent->right;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = _COMiC_RedBlackTree_BLACK;
                pointer->right->color = _COMiC_RedBlackTree_BLACK;
                _COMiC_RedBlackTree_RotateLeft(self, _COMiC_RedBlackTree_CalcParentChildP(self, fix_node_parent), fix_node_parent);
                break;
            }
        }
        else
        {
            pointer = fix_node_parent->left;
            if (pointer->color == _COMiC_RedBlackTree_RED)
            {
                pointer->color = _COMiC_RedBlackTree_BLACK;
                fix_node_parent->color = _COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateRight(self, _COMiC_RedBlackTree_CalcParentChildP(self, fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->left;
            }

            if (pointer == &(self->sentinel))
            { fix_node = fix_node_parent; }
            else if (pointer->right->color == _COMiC_RedBlackTree_BLACK && pointer->left->color == _COMiC_RedBlackTree_BLACK)
            {
                pointer->color = _COMiC_RedBlackTree_RED;
                fix_node = fix_node_parent;
            }
            else
            {
                if (pointer->left->color == _COMiC_RedBlackTree_BLACK)
                {
                    pointer->right->color = _COMiC_RedBlackTree_BLACK;
                    pointer->color = _COMiC_RedBlackTree_RED;
                    _COMiC_RedBlackTree_RotateLeft(self, _COMiC_RedBlackTree_CalcParentChildP(self, pointer), pointer);
                    pointer = fix_node_parent->left;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = _COMiC_RedBlackTree_BLACK;
                pointer->left->color = _COMiC_RedBlackTree_BLACK;
                _COMiC_RedBlackTree_RotateRight(self, _COMiC_RedBlackTree_CalcParentChildP(self, fix_node_parent), fix_node_parent);
                break;
            }
        }
    }

    NOT_BLACK:
    fix_node->color = _COMiC_RedBlackTree_BLACK;
}

static constexpr inline void _COMiC_RedBlackTree_LinkInsteadOf(
        COMiC_InOut COMiC_RedBlackTree *tree,
        COMiC_InOut COMiC_RedBlackTree_Node *parent,
        COMiC_InOut COMiC_RedBlackTree_Node **parent_child_p,
        COMiC_InOut COMiC_RedBlackTree_Node *node,
        COMiC_In COMiC_RedBlackTree_Node *destination
)
{
    if (node == destination)
    { return; }

    *parent_child_p = node;
    node->parent = parent;

    COMiC_RedBlackTree_Node *temp = destination->left;
    node->left = temp;
    if (temp != &(tree->sentinel))
    { temp->parent = node; }

    temp = destination->right;
    node->right = temp;
    if (temp != &(tree->sentinel))
    { temp->parent = node; }

    node->color = destination->color;

#if 0
    destination->parent =  &(tree->sentinel);
    destination->left =  &(tree->sentinel);
    destination->right =  &(tree->sentinel);
#endif
}


typedef struct COMiC_RedBlackTree_SortedIterator
{
    COMiC_RedBlackTree *tree;
    COMiC_RedBlackTree_Node *pos;
} COMiC_RedBlackTree_SortedIterator;

COMiC_Constructor
static constexpr inline void COMiC_RedBlackTree_SortedIterator_Init(
        COMiC_Out COMiC_RedBlackTree_SortedIterator *self,
        COMiC_In COMiC_RedBlackTree *tree
)
{
    self->tree = tree;
    self->pos = _COMiC_RedBlackTree_MinSubNode(tree, self->tree->sentinel.parent);
}

COMiC_Destructor
static constexpr inline void COMiC_RedBlackTree_SortedIterator_Finalize(
        COMiC_In COMiC_RedBlackTree_SortedIterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_RedBlackTree_SortedIterator_Next(
        COMiC_InOut COMiC_RedBlackTree_SortedIterator *self,
        COMiC_Out COMiC_RedBlackTree_Node **pos
)
{
    *pos = self->pos == &(self->tree->sentinel) ? NULL : self->pos;

    if (self->pos->right != &(self->tree->sentinel))
    { self->pos = _COMiC_RedBlackTree_MinSubNode(self->tree, self->pos->right); }
    else
    {
        while (self->pos != self->pos->parent->left && self->pos != &(self->tree->sentinel))
        { self->pos = self->pos->parent; }
    }
}


typedef struct COMiC_RedBlackTree_ReversedSortedIterator
{
    COMiC_RedBlackTree *tree;
    COMiC_RedBlackTree_Node *pos;
} COMiC_RedBlackTree_ReversedSortedIterator;

COMiC_Constructor
static constexpr inline void COMiC_RedBlackTree_ReversedSortedIterator_Init(
        COMiC_Out COMiC_RedBlackTree_ReversedSortedIterator *self,
        COMiC_In COMiC_RedBlackTree *tree
)
{
    self->tree = tree;
    self->pos = _COMiC_RedBlackTree_MaxSubNode(tree, self->tree->sentinel.parent);
}

COMiC_Destructor
static constexpr inline void COMiC_RedBlackTree_ReversedSortedIterator_Finalize(
        COMiC_In COMiC_RedBlackTree_ReversedSortedIterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_RedBlackTree_ReversedSortedIterator_Next(
        COMiC_InOut COMiC_RedBlackTree_ReversedSortedIterator *self,
        COMiC_Out COMiC_RedBlackTree_Node **pos
)
{
    *pos = self->pos == &(self->tree->sentinel) ? NULL : self->pos;

    if (self->pos->left != &(self->tree->sentinel))
    { self->pos = _COMiC_RedBlackTree_MaxSubNode(self->tree, self->pos->left); }
    else
    {
        while (self->pos != self->pos->parent->right && self->pos != &(self->tree->sentinel))
        { self->pos = self->pos->parent; }
    }
}


static constexpr inline COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_DeepestLeftSubNode(
        COMiC_In COMiC_RedBlackTree *tree,
        COMiC_In COMiC_RedBlackTree_Node *node
)
{
    while (true)
    {
        if (node->left != &(tree->sentinel))
        { node = node->left; }
        else if (node->right != &(tree->sentinel))
        { node = node->right; }
        else
        { return node; }
    }
}

static constexpr inline COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_DeepestRightSubNode(
        COMiC_In COMiC_RedBlackTree *tree,
        COMiC_In COMiC_RedBlackTree_Node *node
)
{
    while (true)
    {
        if (node->right != &(tree->sentinel))
        { node = node->right; }
        else if (node->left != &(tree->sentinel))
        { node = node->left; }
        else
        { return node; }
    }
}


typedef struct COMiC_RedBlackTree_FastFreeIterator
{
    COMiC_RedBlackTree *tree;
    COMiC_RedBlackTree_Node *pos;
} COMiC_RedBlackTree_FastFreeIterator;


COMiC_Constructor
static constexpr inline void COMiC_RedBlackTree_FastFreeIterator_Init(
        COMiC_Out COMiC_RedBlackTree_FastFreeIterator *self,
        COMiC_In COMiC_RedBlackTree *tree
)
{
    self->tree = tree;
    self->pos = _COMiC_RedBlackTree_DeepestLeftSubNode(tree, self->tree->sentinel.parent);
}

COMiC_Destructor
static constexpr inline void COMiC_RedBlackTree_FastFreeIterator_Finalize(
        COMiC_In COMiC_RedBlackTree_FastFreeIterator *self
)
{
    COMiC_UnusedArg(self);
}

static constexpr inline void COMiC_RedBlackTree_FastFreeIterator_Next(
        COMiC_InOut COMiC_RedBlackTree_FastFreeIterator *self,
        COMiC_Out COMiC_RedBlackTree_Node **pos
)
{
    *pos = self->pos == &(self->tree->sentinel) ? NULL : self->pos;

    self->pos = self->pos->parent;
    if (self->pos->right != &(self->tree->sentinel))
    { self->pos = _COMiC_RedBlackTree_DeepestLeftSubNode(self->tree, self->pos->right); }
}

# ifdef __cplusplus
}
#endif

#endif /* COMiC_Core_RB_TREE_H */
