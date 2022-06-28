//#include <COMiC/rb_tree.hpp>
#include <COMiC/core/rb_tree.hpp>
#include <iostream>
#include <map>
#include <cstdlib>
#define arr_size (6000000)

int main()
{
    clock_t t = clock();
#if 0
    static COMiC_RedBlackTree_Node nodes[arr_size];
    std::map<int, COMiC_RedBlackTree_Node*> tree;

    for (int i = 0; i < arr_size; i++)
    {
        tree[i] = &(nodes[i]);
    }

#else
    COMiC_RedBlackTree tree{nullptr};
    static COMiC_RedBlackTree_Node nodes[arr_size];

    COMiC_RedBlackTree_Node *p;
    for (auto & node : nodes)
    {
        p = &node;
        _COMiC_RedBlackTree::Link<_COMiC_RedBlackTree::DefaultNode_PointersWrapper>(
                _COMiC_RedBlackTree::DefaultTree_Wrapper<_COMiC_RedBlackTree::DefaultNode_PointersWrapper>(&tree),
                nullptr,
                &p
        );
    }
#endif
    std::cout << ((long double)(clock() - t)) / CLOCKS_PER_SEC << std::endl;
    int a = 1;
}