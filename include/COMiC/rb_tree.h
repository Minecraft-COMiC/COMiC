#ifndef COMiC_RB_TREE_H
# define COMiC_RB_TREE_H

# include <COMiC/types.h>
# include <COMiC/error.h>

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
    struct COMiC_RedBlackTree_Node *root;
} COMiC_RedBlackTree;

static inline COMiC_IfError COMiC_RedBlackTree_Init(
        COMiC_Out COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error
)
{ self->root = NULL; }

static inline COMiC_Bool COMiC_RedBlackTree_IsEmpty(
        COMiC_In COMiC_RedBlackTree *self
)
{ return self->root == NULL; }


# ifdef __cplusplus
};
#endif

#endif /* COMiC_RB_TREE_H */
