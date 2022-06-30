#ifndef COMiC_Core_RB_TREE_H
# define COMiC_Core_RB_TREE_H

# include "COMiC/core/types.h"
# include "error.h"

# ifdef __cplusplus
extern "C" {
#endif

enum COMiC_RedBlackTree_Color
{
    COMiC_RedBlackTree_BLACK = 0,
    COMiC_RedBlackTree_RED = 1
};

struct COMiC_RedBlackTree_Node
{
    enum COMiC_RedBlackTree_Color color;
    struct COMiC_RedBlackTree_Node *parent;
    struct COMiC_RedBlackTree_Node *left;
    struct COMiC_RedBlackTree_Node *right;
};

typedef struct COMiC_RedBlackTree
{
    COMiC_RedBlackTree_Node sentinel;
} COMiC_RedBlackTree;

static constexpr inline COMiC_IfError COMiC_RedBlackTree_Init(
        COMiC_Out COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error
) noexcept
{
    self->sentinel.color = COMiC_RedBlackTree_BLACK;
    self->sentinel.parent = &(self->sentinel);
    self->sentinel.left = nullptr;
    self->sentinel.right = nullptr;
    return COMiC_SUCCESS;
}

static constexpr inline COMiC_Bool COMiC_RedBlackTree_IsEmpty(
        COMiC_In COMiC_RedBlackTree *self
) noexcept
{ return self->sentinel.parent == &(self->sentinel); }


# ifdef __cplusplus
};
#endif

#endif /* COMiC_Core_RB_TREE_H */
