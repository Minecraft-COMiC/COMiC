#include "rb_tree.h"

static constexpr COMiC_RedBlackTree_Node **_COMiC_RedBlackTree_CalcParentChildP(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *node) noexcept
{
    if (node == node->parent->right)
    { return &(node->parent->right); }
    else if (node == node->parent->left)
    { return &(node->parent->left); }
    else
    { return &(tree->root); }
}


static constexpr COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_PrevSubKey(COMiC_RedBlackTree_Node *node) noexcept
{
    while (node->right != nullptr)
    { node = node->right; }
    return node;
}


static constexpr COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_NextSubKey(COMiC_RedBlackTree_Node *node) noexcept
{
    while (node->left != nullptr)
    { node = node->left; }
    return node;
}


void constexpr _COMiC_RedBlackTree_RotateLeft(COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *top) noexcept
{
    COMiC_RedBlackTree_Node *node = top->right;
    top->right = node->left;

    if (node->left != nullptr)
    { node->left->parent = top; }

    node->parent = top->parent;

    *parent_child_p = node;

    node->left = top;
    top->right = node;
}

void constexpr _COMiC_RedBlackTree_RotateRight(COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *top) noexcept
{
    COMiC_RedBlackTree_Node *node = top->left;
    top->left = node->right;

    if (node->right == nullptr)
    { node->right->parent = top; }

    node->parent = top->parent;

    *parent_child_p = node;

    node->right = top;
    top->parent = node;
}


COMiC_RedBlackTree_Node *_COMiC_RedBlackTree_Link(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *parent, COMiC_RedBlackTree_Node **parent_child_p, COMiC_RedBlackTree_Node *node) noexcept
{
    COMiC_RedBlackTree_Node *parent_sibling;
    COMiC_RedBlackTree_Node *pointer;

    node->parent = parent;
    if (parent == nullptr)
    {
        node->color = COMiC_RedBlackTree_BLACK; // the root is black
        return nullptr;
    }
    else
    { *parent_child_p = node; }

    pointer = node;
    while (pointer->parent->color == COMiC_RedBlackTree_RED)
    {
        if (pointer->parent == pointer->parent->parent->left)
        {
            parent_sibling = pointer->parent->parent->right;
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
                    _COMiC_RedBlackTree_RotateLeft(&(pointer->parent->left), pointer);
                }

                pointer->parent->color = COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateRight(_COMiC_RedBlackTree_CalcParentChildP(tree, pointer->parent->parent), pointer->parent->parent);
            }
        }
        else
        {
            parent_sibling = pointer->parent->parent->right;
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
                    _COMiC_RedBlackTree_RotateLeft(&(pointer->parent->right), pointer);
                }

                pointer->parent->color = COMiC_RedBlackTree_BLACK;
                pointer->parent->parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateLeft(_COMiC_RedBlackTree_CalcParentChildP(tree, pointer->parent->parent), pointer->parent->parent);
            }
        }
    }

    tree->root->color = COMiC_RedBlackTree_BLACK;
    return node;
}

COMiC_IfError _COMiC_RedBlackTree_UnLink(COMiC_RedBlackTree *tree, COMiC_RedBlackTree_Node *node) noexcept
{
    COMiC_RedBlackTree_Node *fix_node; // the node to recolor as needed
    COMiC_RedBlackTree_Node *fix_node_parent; // parent of fix_node (which may be nil)
    COMiC_RedBlackTree_Node *pointer = node;

    if (pointer->left == nullptr)
    { fix_node = pointer->right; }
    else if (pointer->right == nullptr)
    { fix_node = pointer->left; }
    else
    {
        pointer = _COMiC_RedBlackTree_NextSubKey(node->right);
        fix_node = pointer->right;
        goto TWO_CHILDREN;
    }

    fix_node_parent = node->parent;
    if (fix_node != nullptr)
    { fix_node->parent = fix_node_parent; }

    if (node->parent == nullptr)
    { tree->root = fix_node; }
    else if (fix_node_parent->left == node)
    { fix_node_parent->left = fix_node; }
    else
    { fix_node_parent->right = fix_node; }

#   if 0
    if (tree->lowest == node)
    { tree->lowest = (fix_node == nullptr) ? fix_node_parent : _COMiC_RedBlackTree_PrevSubKey(fix_node); }

    if (tree->greatest == node)
    { tree->greatest = (fix_node == nullptr) ? fix_node_parent : _COMiC_RedBlackTree_NextSubKey(fix_node); }
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
        if (fix_node != nullptr)
        { fix_node->parent = fix_node_parent; }

        fix_node_parent->left = fix_node;
        pointer->right = node->right;
        node->right->right = pointer;
    }

    if (node->parent == nullptr)
    { tree->root = pointer; }
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

    while ((fix_node_parent = fix_node->parent) != nullptr && fix_node->color == COMiC_RedBlackTree_BLACK)
    {
        if (fix_node == fix_node_parent->left)
        {
            pointer = fix_node_parent->right;
            if (pointer->color == COMiC_RedBlackTree_RED)
            {
                pointer->color = COMiC_RedBlackTree_BLACK;
                fix_node_parent->color = COMiC_RedBlackTree_RED;
                _COMiC_RedBlackTree_RotateLeft(_COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->right;
            }

            if (pointer == nullptr)
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
                    _COMiC_RedBlackTree_RotateRight(_COMiC_RedBlackTree_CalcParentChildP(tree, pointer), pointer);
                    pointer = fix_node_parent->right;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = COMiC_RedBlackTree_BLACK;
                pointer->right->color = COMiC_RedBlackTree_BLACK;
                _COMiC_RedBlackTree_RotateLeft(_COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
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
                _COMiC_RedBlackTree_RotateRight(_COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->left;
            }

            if (pointer == nullptr)
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
                    _COMiC_RedBlackTree_RotateLeft(_COMiC_RedBlackTree_CalcParentChildP(tree, pointer), pointer);
                    pointer = fix_node_parent->left;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = COMiC_RedBlackTree_BLACK;
                pointer->left->color = COMiC_RedBlackTree_BLACK;
                _COMiC_RedBlackTree_RotateRight(_COMiC_RedBlackTree_CalcParentChildP(tree, fix_node_parent), fix_node_parent);
                break;
            }
        }
    }
    fix_node->color = COMiC_RedBlackTree_BLACK;

    NOT_BLACK:
    return COMiC_SUCCESS;
}
