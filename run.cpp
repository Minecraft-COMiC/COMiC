//#include <COMiC/rb_tree.hpp>
#include "include/COMiC/rb_tree.hpp"

int main()
{
    COMiC_RedBlackTree tree{nullptr};
   static COMiC_RedBlackTree_Node nodes[10000000];

    COMiC_RedBlackTree_Node *p;
    for (int i = 0; i < 10000000; i++)
    {
        p = &(nodes[i]);
        _COMiC_RedBlackTree::Link<_COMiC_RedBlackTree::DefaultNode_PointersWrapper>(
                _COMiC_RedBlackTree::DefaultTree_Wrapper<_COMiC_RedBlackTree::DefaultNode_PointersWrapper>(&tree),
                nullptr,
                &p
                );
    }
    int a = 1;
}