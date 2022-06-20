#include <COMiC.h>

struct COMiC_GlobalHeap_MapVertex
{
    struct COMiC_RedBlackTree_Node vertex;
    void *block;
};

COMiC_RedBlackTree_SelectorResult COMiC_GlobalHeap_MapSelector(
        COMiC_In void **data,
        COMiC_In const struct COMiC_GlobalHeap_MapVertex *vertex
)
{
    if (vertex->block == *data)
    { return COMiC_RedBlackTree_SELECT; }
    if (((COMiC_UIntPtr) *data) < ((COMiC_UIntPtr) vertex->block))
    { return COMiC_RedBlackTree_GO_LEFT; }
    else
    { return COMiC_RedBlackTree_GO_RIGHT; }

}


COMiC_Constructor
COMiC_IfError COMiC_GlobalHeap_Init(
        COMiC_Out COMiC_GlobalHeap *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_heap_size
)
{
    if (COMiC_RedBlackTree_Init(
            &(self->map),
            error,
            self,
            (COMiC_RedBlackTree_Selector) COMiC_GlobalHeap_MapSelector,
            sizeof(struct COMiC_GlobalHeap_MapVertex)
    )) {
        return COMiC_ERROR;
    }
}
