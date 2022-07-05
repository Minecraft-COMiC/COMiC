#ifndef COMiC_Core_ORDERED_RB_TREE_HPP
# define COMiC_Core_ORDERED_RB_TREE_HPP

# include "rb_tree.hpp"
# include "ordered_rb_tree.h"

template<class key_wrapper_t>
inline COMiC_IfError _COMiC_OrderedRedBlackTree_Find(
        COMiC_InOut COMiC_OrderedRedBlackTree *tree,
        COMiC_In key_wrapper_t key,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **parent,
        COMiC_Out COMiC_RedBlackTree_Node ***parent_child_p
) noexcept
{ return _COMiC_RedBlackTree_Find((COMiC_RedBlackTree *) tree, key, (COMiC_RedBlackTree_Node **) parent, (COMiC_RedBlackTree_Node ***) parent_child_p); }

template<class key_wrapper_t>
inline COMiC_IfError COMiC_OrderedRedBlackTree_Find7Link(
        COMiC_InOut COMiC_OrderedRedBlackTree *tree,
        COMiC_In key_wrapper_t key,
        COMiC_InOut COMiC_OrderedRedBlackTree_Node **node
) noexcept
{
    COMiC_OrderedRedBlackTree_Node *parent;
    COMiC_OrderedRedBlackTree_Node **parent_child_p;
    if (_COMiC_OrderedRedBlackTree_Find(tree, key, &parent, &parent_child_p))
    { return COMiC_ERROR; }

    if (*parent_child_p != (COMiC_OrderedRedBlackTree_Node *) &(tree->tree.sentinel))
    {
        COMiC_OrderedRedBlackTree_Node *old = *parent_child_p;
        _COMiC_OrderedRedBlackTree_LinkInsteadOf(tree, parent, parent_child_p, *node, *parent_child_p);
        *node = old;
        return COMiC_SUCCESS;
    }

    COMiC_OrderedRedBlackTree_Link(tree, parent, parent_child_p, *node);
    return COMiC_SUCCESS;
}

#endif /* COMiC_Core_ORDERED_RB_TREE_HPP */