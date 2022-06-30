#include <COMiC/core/rb_tree.hpp>
#include <iostream>
#include <map>
#include <cstdlib>
#define arr_size (60)

void my_func()
{}

int main();

void _start()
{
    main();
}

int main()
{
    clock_t t = clock();
    my_func();
#if 1
    static int nodes[arr_size];
    std::map<int, void *> tree;

    for (int i = 0; i < arr_size; i++)
    {
        tree.insert({i, (void *)(uintptr_t)i});
    }
    for (int i = 31; i < arr_size; i++)
    {
        tree.erase(i);
    }
#else
    COMiC_RedBlackTree tree{nullptr};
    static COMiC_RedBlackTree_Node nodes[arr_size];

    COMiC_RedBlackTree_Node *p;
    for (int i = 0; i < arr_size; i++)
    {
        p = &(nodes[i]);
        _COMiC_RedBlackTree::Link<_COMiC_RedBlackTree::DefaultNode_PointersWrapper>(
                _COMiC_RedBlackTree::DefaultTree_Wrapper<_COMiC_RedBlackTree::DefaultNode_PointersWrapper>(&tree),
                nullptr,
                &p
        );
    }
#endif
    std::cout << ((long double)(clock() - t)) / CLOCKS_PER_SEC << std::endl;
    int a = 1;
    return 0;
}