#include <COMiC.h>

#define COMiC_RedBlackTree_GetVertexValue(VERTEX) ((void *)(((COMiC_UIntPtr)(VERTEX)) + sizeof(struct COMiC_RedBlackTree_Node)))

COMiC_Constructor
COMiC_IfError COMiC_RedBlackTree_Init(
        COMiC_Out COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_RedBlackTree_Selector selector
)
{
    self->selector = selector;
    self->root = NULL;
    return COMiC_SUCCESS;
}

COMiC_Destructor
COMiC_IfError COMiC_RedBlackTree_Finalize(
        COMiC_In COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error
)
{
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_RedBlackTree_Link(
        COMiC_In COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In struct COMiC_RedBlackTree_Node *vertex
)
{
    struct COMiC_RedBlackTree_Node *p = self->root;
    struct COMiC_RedBlackTree_Node *q;

    vertex->left = NULL;
    vertex->right = NULL;
    if (p == NULL)
    {
        vertex->parent = NULL;
        self->root = NULL;
        vertex->color = COMiC_RedBlackTree_BLACK;
        return COMiC_SUCCESS;
    }
    else
    {
        vertex->color = COMiC_RedBlackTree_RED;
        q = NULL;
        while (p != NULL)
        {
            q = p;
            switch (self->selector(
                    error,
                    COMiC_RedBlackTree_GetVertexValue(vertex),
                    COMiC_RedBlackTree_GetVertexValue(p)
            ))
            {

                case COMiC_RedBlackTree_ERROR:
                    return COMiC_ERROR;
                case COMiC_RedBlackTree_SELECT:
                case COMiC_RedBlackTree_GO_LEFT:
                    p = p->left;
                    break;
                case COMiC_RedBlackTree_GO_RIGHT:
                    p = p->right;
                    break;
            }
        }
        vertex->parent = q;
        switch (self->selector(
                error,
                COMiC_RedBlackTree_GetVertexValue(vertex),
                COMiC_RedBlackTree_GetVertexValue(q)
        ))
        {

            case COMiC_RedBlackTree_ERROR:
                return COMiC_ERROR;
            case COMiC_RedBlackTree_SELECT:
            case COMiC_RedBlackTree_GO_LEFT:
                q->left = p;
                break;
            case COMiC_RedBlackTree_GO_RIGHT:
                q->right = p;
                break;
        }
    }

    p = vertex;
    /* balancing */
    {
        register struct COMiC_RedBlackTree_Node *parent;
        register struct COMiC_RedBlackTree_Node *grandparent;
        register struct COMiC_RedBlackTree_Node *uncle;

        while (p->color == COMiC_RedBlackTree_RED)
        {
            parent = p->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left)
            {
                if (grandparent->right != NULL && p->parent->parent->right->color == COMiC_RedBlackTree_RED)
                {
                    p->parent->color = COMiC_RedBlackTree_BLACK;
                    p->parent->parent->color = COMiC_RedBlackTree_RED;
                    p->parent->parent->right->color = COMiC_RedBlackTree_BLACK;
                    p = p->parent->parent;
                }
            }
        }
    }

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_RedBlackTree_UnLink(
        COMiC_In COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In struct COMiC_RedBlackTree_Node *vertex
)
{

}

COMiC_IfError COMiC_RedBlackTree_Find(
        COMiC_In COMiC_RedBlackTree *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In const void *key
);