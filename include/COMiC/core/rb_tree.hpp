#ifndef COMiC_Core_RB_TREE_HPP
# define COMiC_Core_RB_TREE_HPP

# ifndef __cplusplus
#  error This header is only for C++
# endif

#include "rb_tree.h"

template<class key_wrapper_t>
inline COMiC_IfError _COMiC_RedBlackTree_Find(
        COMiC_RedBlackTree *tree,
        key_wrapper_t key,
        COMiC_RedBlackTree_Node **parent,
        COMiC_RedBlackTree_Node ***parent_child_p
) noexcept
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

template<class key_wrapper_t>
inline COMiC_IfError COMiC_RedBlackTree_Link7Link(
        COMiC_RedBlackTree *tree,
        key_wrapper_t key,
        COMiC_RedBlackTree_Node **node
) noexcept
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

    if (COMiC_RedBlackTree_Link(tree, parent, parent_child_p, *node))
    { return COMiC_ERROR; }
    return COMiC_SUCCESS;
}

#endif /* COMiC_Core_RB_TREE_HPP */