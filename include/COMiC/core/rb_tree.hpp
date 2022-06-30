#include "rb_tree.h"

static inline constexpr COMiC_RedBlackTree_Node **_COMiC_RedBlackTree_CalcParentChildP(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *node) noexcept
{
    if (node == node->parent->right)
    { return &(node->parent->right); }
    else if (node == node->parent->left)
    { return &(node->parent->left); }
    else
    { return &(tree->sentinel.parent); }
}


static inline constexpr COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_PrevSubKey(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *node) noexcept
{
    while (node->right != &(tree->sentinel))
    { node = node->right; }
    return node;
}


static inline constexpr COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_NextSubKey(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *node) noexcept
{
    while (node->left != &(tree->sentinel))
    { node = node->left; }
    return node;
}


void inline constexpr _COMiC_RedBlackTree_RotateLeft(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *top) noexcept
{
    COMiC_RedBlackTree_Node *node = top->right;
    top->right = node->left;

    if (node->left != &(tree->sentinel))
    { node->left->parent = top; }

    node->parent = top->parent;

    *parent_child_p = node;

    node->left = top;
    top->parent = node;
}


inline constexpr void _COMiC_RedBlackTree_RotateRight(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *top) noexcept
{
    COMiC_RedBlackTree_Node *node = top->left;
    top->left = node->right;

    if (node->right != &(tree->sentinel))
    { node->right->parent = top; }

    node->parent = top->parent;

    *parent_child_p = node;

    node->right = top;
    top->parent = node;
}

template<class key_wrapper_t>
inline COMiC_IfError _COMiC_RedBlackTree_Find(COMiC_RedBlackTree *tree, key_wrapper_t key, COMiC_RedBlackTree_Node **parent, COMiC_RedBlackTree_Node ***parent_child_p)
{
    COMiC_ComparisonResult comparison_result;

    *parent = &((*tree).sentinel);
    *parent_child_p = &((*parent)->parent);
    while (**parent_child_p != &(tree->sentinel))
    {
        *parent = **parent_child_p;
        if (key.compare_to(**parent_child_p, &comparison_result))
        { return COMiC_ERROR; }

        if (comparison_result > 0)
        { *parent_child_p = &((**parent_child_p)->right); }
        else if (comparison_result < 0)
        { *parent_child_p = &((**parent_child_p)->left); }
        else
        { break; }
    }
    return COMiC_SUCCESS;
}

inline constexpr COMiC_IfError _COMiC_RedBlackTree_Link(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *parent, COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *node) noexcept
{
    node->parent = parent;
    *parent_child_p = node;
    node->left = &(tree->sentinel);
    node->right = &(tree->sentinel);
    if (parent == &(tree->sentinel))
    {
        node->color = COMiC_RedBlackTree_BLACK;
        return COMiC_SUCCESS;
    }
    node->color = COMiC_RedBlackTree_RED;

    COMiC_RedBlackTree_Node *pointer = node;
    while (pointer->parent->color == COMiC_RedBlackTree_RED)
    {
        if (pointer->parent == pointer->parent->parent->left)
        {
            COMiC_RedBlackTree_Node *parent_sibling = pointer->parent->parent->right;
            if (parent_sibling->color == COMiC_RedBlackTree_RED)
            {
                pointer->parent->color = COMiC_RedBlackTree_BLACK;
                parent_sibling->color = COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = COMiC_RedBlackTree_RED;
                pointer = pointer->parent->parent;
            }
            else
            {
                if (pointer == pointer->parent->right)
                {
                    pointer = pointer->parent;
                    _COMiC_RedBlackTree_RotateLeft(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, pointer), pointer);
                }

                pointer->parent->color = COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateRight(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, pointer->parent->parent), pointer->parent->parent);
            }
        }
        else
        {
            COMiC_RedBlackTree_Node *parent_sibling = pointer->parent->parent->left;
            if (parent_sibling->color == COMiC_RedBlackTree_RED)
            {
                pointer->parent->color = COMiC_RedBlackTree_BLACK;
                parent_sibling->color = COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = COMiC_RedBlackTree_RED;
                pointer = pointer->parent->parent;
            }
            else
            {
                if (pointer == pointer->parent->left)
                {
                    pointer = pointer->parent;
                    _COMiC_RedBlackTree_RotateLeft(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, pointer), pointer);
                }

                pointer->parent->color = COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateLeft(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, pointer->parent->parent), pointer->parent->parent);
            }
        }
    }

    tree->sentinel.parent->color = COMiC_RedBlackTree_BLACK;
    return COMiC_SUCCESS;
}

static inline constexpr void _COMiC_RedBlackTree_LinkInsteadOf(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *parent, COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *node, COMiC_RedBlackTree_Node *destination)
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

template<class key_wrapper_t>
inline COMiC_IfError COMiC_RedBlackTree_Link(COMiC_RedBlackTree *tree, key_wrapper_t key, COMiC_RedBlackTree_Node **node) noexcept
{
    COMiC_RedBlackTree_Node *parent;
    COMiC_RedBlackTree_Node **parent_child_p;
    if (_COMiC_RedBlackTree_Find(tree, key, &parent, &parent_child_p))
    { return COMiC_ERROR; }

    if (*parent_child_p != &(tree->sentinel))
    {
        COMiC_RedBlackTree_Node *old = *parent_child_p;
        _COMiC_RedBlackTree_LinkInsteadOf(tree, parent, parent_child_p, *node, *parent_child_p);
        *node = old;
        return COMiC_SUCCESS;
    }

    if (_COMiC_RedBlackTree_Link(tree, parent, parent_child_p, *node))
    { return COMiC_ERROR; }
    return COMiC_SUCCESS;

}

COMiC_IfError COMiC_RedBlackTree_UnLink(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *node) noexcept
{
    COMiC_RedBlackTree_Node *fix_node;
    COMiC_RedBlackTree_Node *fix_node_parent;
    COMiC_RedBlackTree_Node *pointer = node;

    if (pointer->left == &(tree->sentinel))
    { fix_node = pointer->right; }
    else if (pointer->right == &(tree->sentinel))
    { fix_node = pointer->left; }
    else
    {
        pointer = _COMiC_RedBlackTree_NextSubKey(tree, node->right);
        fix_node = pointer->right;
        goto TWO_CHILDREN;
    }

    fix_node_parent = node->parent;
    if (fix_node != &(tree->sentinel))
    { fix_node->parent = fix_node_parent; }

    if (node->parent == &(tree->sentinel))
    { tree->sentinel.parent = fix_node; }
    else if (fix_node_parent->left == node)
    { fix_node_parent->left = fix_node; }
    else
    { fix_node_parent->right = fix_node; }

#   if 0
    if (tree->lowest == node)
    { tree->lowest = (fix_node ==  &(tree->sentinel)) ? fix_node_parent : _COMiC_RedBlackTree_PrevSubKey(fix_node); }

    if (tree->greatest == node)
    { tree->greatest = (fix_node ==  &(tree->sentinel)) ? fix_node_parent : _COMiC_RedBlackTree_NextSubKey(fix_node); }
#   endif

    if (node->color == COMiC_RedBlackTree_RED)
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
        if (fix_node != &(tree->sentinel))
        { fix_node->parent = fix_node_parent; }

        fix_node_parent->left = fix_node;
        pointer->right = node->right;
        node->right->right = pointer;
    }

    if (node->parent == &(tree->sentinel))
    { tree->sentinel.parent = pointer; }
    else if (node->parent->left == node)
    { node->parent->left = pointer; }
    else
    { node->parent->right = pointer; }

    pointer->parent = node->parent;
    if (node->color == COMiC_RedBlackTree_RED)
    { goto NOT_BLACK; }
    node->color = pointer->color;
    pointer->color = COMiC_RedBlackTree_BLACK;
    BALANCE:

    while ((fix_node_parent = fix_node->parent) != &(tree->sentinel) && fix_node->color == COMiC_RedBlackTree_BLACK)
    {
        if (fix_node == fix_node_parent->left)
        {
            pointer = fix_node_parent->right;
            if (pointer->color == COMiC_RedBlackTree_RED)
            {
                pointer->color = COMiC_RedBlackTree_BLACK;
                fix_node_parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateLeft(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->right;
            }

            if (pointer == &(tree->sentinel))
            { fix_node = fix_node_parent; }
            else if (pointer->left->color == COMiC_RedBlackTree_BLACK && pointer->right->color == COMiC_RedBlackTree_BLACK)
            {
                pointer->color = COMiC_RedBlackTree_RED;
                fix_node = fix_node_parent;
            }
            else
            {
                if (pointer->right->color == COMiC_RedBlackTree_BLACK)
                {
                    pointer->left->color = COMiC_RedBlackTree_BLACK;
                    pointer->color = COMiC_RedBlackTree_RED;
                    _COMiC_RedBlackTree_RotateRight(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, pointer), pointer);
                    pointer = fix_node_parent->right;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = COMiC_RedBlackTree_BLACK;
                pointer->right->color = COMiC_RedBlackTree_BLACK;
                _COMiC_RedBlackTree_RotateLeft(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                break;
            }
        }
        else
        {
            pointer = fix_node_parent->left;
            if (pointer->color == COMiC_RedBlackTree_RED)
            {
                pointer->color = COMiC_RedBlackTree_BLACK;
                fix_node_parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateRight(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->left;
            }

            if (pointer == &(tree->sentinel))
            { fix_node = fix_node_parent; }
            else if (pointer->right->color == COMiC_RedBlackTree_BLACK && pointer->left->color == COMiC_RedBlackTree_BLACK)
            {
                pointer->color = COMiC_RedBlackTree_RED;
                fix_node = fix_node_parent;
            }
            else
            {
                if (pointer->left->color == COMiC_RedBlackTree_BLACK)
                {
                    pointer->right->color = COMiC_RedBlackTree_BLACK;
                    pointer->color = COMiC_RedBlackTree_RED;
                    _COMiC_RedBlackTree_RotateLeft(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, pointer), pointer);
                    pointer = fix_node_parent->left;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = COMiC_RedBlackTree_BLACK;
                pointer->left->color = COMiC_RedBlackTree_BLACK;
                _COMiC_RedBlackTree_RotateRight(tree, _COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                break;
            }
        }
    }
    fix_node->color = COMiC_RedBlackTree_BLACK;

    NOT_BLACK:
    return COMiC_SUCCESS;
}
