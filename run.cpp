#include <COMiC/core/rb_tree.hpp>
#include <COMiC/core/arena.hpp>
#include <COMiC/core/ordered_rb_tree.hpp>
#include <iostream>
#include <map>
#include <cstdlib>

#define arr_size (60000000)

void my_func()
{}

int main();

void _start()
{
    main();
}

class key
{
private:
    COMiC_RedBlackTree_Node *p;
public:
    explicit key(COMiC_RedBlackTree_Node *p) : p(p)
    {}

    COMiC_IfError compare_to(COMiC_RedBlackTree_Node *o, COMiC_ComparisonResult *r)
    {
        if (this->p < o)
        { *r = COMiC_LESS; }
        else if (this->p > o)
        { *r = COMiC_GREATER; }
        else
        { *r = COMiC_EQUALS; }
        return COMiC_SUCCESS;
    }
};

int main()
{
    _COMiC_Arena_OccupancyInfo f;
    clock_t t = clock();
    COMiC_Arena<int> x;
    my_func();
    static COMiC_RedBlackTree_Node nodes[arr_size];
#if 0
    std::map<COMiC_RedBlackTree_Node *, int> tree;

    for (int i = 0; i < arr_size; i++)
    {
        tree.insert({&(nodes[i]), i});
    }
//    for (int i = 0; i < arr_size; i++)
//    {
//        tree.erase(&(nodes[i]));
//    }
#else
    COMiC_RedBlackTree tree;
    COMiC_RedBlackTree_Constructor(&tree);

    COMiC_RedBlackTree_Node *p;
    for (int i = 0; i < arr_size; i++)
    {
        p = &(nodes[i]);
        COMiC_RedBlackTree_Find7Link<key>(&tree, key(p), &p);
    }
//    for (int i = 0; i < arr_size; i++)
//    {
//        COMiC_RedBlackTree_UnLink(&tree, &(nodes[i]));
//    }
#endif
    std::cout << ((long double) (clock() - t)) / CLOCKS_PER_SEC << std::endl;
    int a = 1;
    return 0;
}