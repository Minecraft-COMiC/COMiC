#ifndef COMiC_RB_TREE_HPP
# define COMiC_RB_TREE_HPP

#include <cstddef>
#include <COMiC/types.h>
#include <COMiC/error.h>
#include <COMiC/rb_tree.h>

namespace _COMiC_RedBlackTree
{
    /* Wrapper classes for rotation functions delegates assigning child of node that was rotated */
    namespace _AsParent_Wrapper
    {
        template<class node_wrapper_t>
        class NodeLeft_CN
        {
        private:
            node_wrapper_t _real;
        public:
            constexpr inline explicit NodeLeft_CN(node_wrapper_t real) noexcept: _real(real)
            {}

            constexpr inline void set_child(node_wrapper_t node) noexcept
            { this->_real.set_left(node); }

            constexpr inline node_wrapper_t real() const noexcept
            { return this->_real; }
        };

        template<class node_wrapper_t>
        class NodeRight_CN
        {
        private:
            node_wrapper_t _real;
        public:
            constexpr inline explicit NodeRight_CN(node_wrapper_t real) noexcept: _real(real)
            {}

            constexpr inline void set_child(node_wrapper_t node) noexcept
            { this->_real.set_right(node); }

            constexpr inline node_wrapper_t real() const noexcept
            { return this->_real; }
        };

        template<class tree_wrapper_t, class node_wrapper_t>
        class Root_CN
        {
        private:
            tree_wrapper_t _real;
        public:
            constexpr inline explicit Root_CN(tree_wrapper_t real) noexcept: _real(real)
            {}

            constexpr inline void set_child(node_wrapper_t node) noexcept
            { this->_real.set_root(node); }


            constexpr inline node_wrapper_t real() const noexcept
            { return node_wrapper_t(nullptr); }
        };
    }

    template<class node_wrapper_t>
    class DefaultTree_Wrapper
    {
    private:
        COMiC_RedBlackTree *const _raw;
    public:
        constexpr inline explicit DefaultTree_Wrapper(COMiC_RedBlackTree *raw) noexcept: _raw(raw)
        {}

        constexpr inline node_wrapper_t get_root() const noexcept
        { return node_wrapper_t(this->_raw->root); }

        constexpr inline void set_root(COMiC_RedBlackTree_Node *node) const noexcept
        { this->_raw->root = node; }

        constexpr inline void set_root(node_wrapper_t node) const noexcept
        { this->_raw->root = node.raw(); }
    };

    class DefaultNode_PointersWrapper
    {
    private:
        COMiC_RedBlackTree_Node *_raw;

    public:

        inline constexpr explicit DefaultNode_PointersWrapper(COMiC_RedBlackTree_Node *raw) noexcept: _raw(raw)
        {}

        inline constexpr DefaultNode_PointersWrapper(DefaultNode_PointersWrapper const &raw) noexcept = default;

        inline constexpr DefaultNode_PointersWrapper get_parent() const noexcept
        { return DefaultNode_PointersWrapper(this->_raw->parent); }

        inline constexpr void set_parent(DefaultNode_PointersWrapper node) noexcept
        { this->_raw->parent = node._raw; }

        inline constexpr void set_parent(COMiC_RedBlackTree_Node *node) noexcept
        { this->_raw->parent = node; }

        inline constexpr DefaultNode_PointersWrapper get_left() const noexcept
        { return DefaultNode_PointersWrapper(this->_raw->left); }

        inline constexpr void set_left(DefaultNode_PointersWrapper node) noexcept
        { this->_raw->left = node._raw; }

        inline constexpr void set_left(COMiC_RedBlackTree_Node *node) noexcept
        { this->_raw->left = node; }

        inline constexpr DefaultNode_PointersWrapper get_right() const noexcept
        { return DefaultNode_PointersWrapper(this->_raw->right); }

        inline constexpr void set_right(DefaultNode_PointersWrapper node) noexcept
        { this->_raw->right = node._raw; }

        inline constexpr void set_right(COMiC_RedBlackTree_Node *node) noexcept
        { this->_raw->right = node; }

        inline constexpr COMiC_Bool operator==(DefaultNode_PointersWrapper other) const noexcept
        { return this->_raw == other._raw; }

        inline constexpr COMiC_Bool operator==(COMiC_RedBlackTree_Node *other) const noexcept
        { return this->_raw == other; }

        inline constexpr COMiC_Bool operator!=(DefaultNode_PointersWrapper other) const noexcept
        { return this->_raw != other._raw; }

        inline constexpr COMiC_Bool operator!=(COMiC_RedBlackTree_Node *other) const noexcept
        { return this->_raw != other; }

        inline constexpr COMiC_RedBlackTree_Node *raw() const noexcept
        { return this->_raw; }

        inline constexpr COMiC_RedBlackTree_Color get_color() const noexcept
        { return this->_raw->color; }

        inline constexpr void set_color(COMiC_RedBlackTree_Color color) noexcept
        { this->_raw->color = color; }

        inline constexpr COMiC_IfError
        compare_to(COMiC_Error *error, DefaultNode_PointersWrapper other, COMiC_ComparisonResult *result) const noexcept
        {
            if (this->_raw < other._raw)
            { *result = COMiC_LESS; }
            else if (this->_raw > other._raw)
            { *result = COMiC_GREATER; }
            else
            { *result = COMiC_EQUALS; }
            return COMiC_SUCCESS;
        }
    };

    template<
            class as_parent_wrapper_t, /* See _COMiC_RedBlackTree::_AsParent_Wrapper namespace */
            class node_wrapper_t /* Delegate to node */
    >
    static inline constexpr void _COMiC_RedBlackTree_RotateLeft(
            COMiC_In as_parent_wrapper_t parent,
            COMiC_In node_wrapper_t node,
            COMiC_In node_wrapper_t child
    )
    {
        child.set_parent(parent.real());
        parent.set_child(child);
        node.set_parent(child);
        node_wrapper_t grandchild = child.get_left();
        node.set_right(grandchild);
        child.set_left(node);
        if (grandchild != nullptr)
        { grandchild.set_parent(node); }
    }

    template<
            class as_parent_wrapper_t, /* See _COMiC_RedBlackTree::_AsParent_Wrapper namespace */
            class node_wrapper_t /* Delegate to node */
    >
    static inline constexpr void _COMiC_RedBlackTree_RotateRight(
            COMiC_In as_parent_wrapper_t parent,
            COMiC_In node_wrapper_t node,
            COMiC_In node_wrapper_t child
    )
    {
        child.set_parent(parent.real());
        parent.set_child(child);
        node.set_parent(child);
        node_wrapper_t grandchild = child.get_right();
        node.set_left(grandchild);
        child.set_right(node);
        if (grandchild != nullptr)
        { grandchild.set_parent(node); }
    }

    template<class node_wrapper_t>
    static inline constexpr void _LinkInsteadOf(
            COMiC_Out node_wrapper_t self,
            COMiC_In node_wrapper_t destination
    )
    {
        if (self == destination)
        { return; }

        node_wrapper_t temp(destination.get_parent());

        self.set_parent(temp);
        if (temp.get_left() == self)
        { temp.set_left(self); }
        else
        { temp.set_right(self); }

        temp = destination.get_left();
        self.set_left(temp);
        if (temp != nullptr)
        { temp.set_parent(self); }

        temp = destination.get_right();
        self.set_right(temp);
        if (temp != nullptr)
        { temp.set_parent(self); }

        self.set_color(destination.get_color());

        destination.set_parent(nullptr);
        destination.set_left(nullptr);
        destination.set_right(nullptr);
    }

    /* https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insertion */
    template<
            class node_wrapper_t,
            class tree_wrapper_t =_COMiC_RedBlackTree::DefaultTree_Wrapper<node_wrapper_t>,
            typename raw_node_t = COMiC_RedBlackTree_Node *,
            class as_parent_wrapper_left_t = _COMiC_RedBlackTree::_AsParent_Wrapper::NodeLeft_CN<node_wrapper_t>,
            class as_parent_wrapper_right_t = _COMiC_RedBlackTree::_AsParent_Wrapper::NodeRight_CN<node_wrapper_t>,
            class as_parent_wrapper_root_t = _COMiC_RedBlackTree::_AsParent_Wrapper::Root_CN<tree_wrapper_t, node_wrapper_t>
    >
    static inline COMiC_IfError Link(
            COMiC_In tree_wrapper_t self,
            COMiC_Out COMiC_Error *const error,
            COMiC_In COMiC_Out raw_node_t *cell
    )
    {
        node_wrapper_t node = node_wrapper_t(*cell);
        node_wrapper_t parent(nullptr);
        node_wrapper_t uncle(nullptr);
        node_wrapper_t grandparent(nullptr);
        COMiC_ComparisonResult last_comparison_result;

        *cell = nullptr;

        node.set_parent(NULL);
        node.set_left(NULL);
        node.set_right(NULL);

        grandparent = self.get_root();
        if (grandparent == nullptr)
        {
            self.set_root(node);
            return COMiC_SUCCESS;
        }

        node.set_color(COMiC_RedBlackTree_RED);

        while (grandparent != nullptr)
        {
            parent = grandparent;
            if (node.compare_to(error, grandparent, &last_comparison_result))
            {}

            switch (last_comparison_result)
            {
                case COMiC_LESS:
                    grandparent = grandparent.get_left();
                    break;
                case COMiC_EQUALS:
                    _COMiC_RedBlackTree::_LinkInsteadOf(node, grandparent);
                    *cell = grandparent.raw();
                    return COMiC_SUCCESS;
                case COMiC_GREATER:
                    grandparent = grandparent.get_right();
                    break;
            }
        }

        switch (last_comparison_result)
        {
            case COMiC_LESS:
                parent.set_left(node);
                break;
            case COMiC_EQUALS:
                /* todo */
                return COMiC_ERROR;
            case COMiC_GREATER:
                parent.set_right(node);
                break;
        }
        node.set_parent(parent);

        do
        {
            /* https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insert_case_1 */
            if (parent.get_color() == COMiC_RedBlackTree_BLACK)
            {
                return COMiC_SUCCESS;
            }
            if ((grandparent = parent.get_parent()) == nullptr)
            {
                /* https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insert_case_4 */
                parent.set_color(COMiC_RedBlackTree_BLACK);
                return COMiC_SUCCESS;
            }

            uncle = grandparent.get_left();
            if (uncle == parent)
            { uncle = grandparent.get_right(); }

            if (uncle == nullptr || uncle.get_color() == COMiC_RedBlackTree_BLACK)
            { goto POST; }

            /* https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insert_case_2 */
            parent.set_color(COMiC_RedBlackTree_BLACK);
            uncle.set_color(COMiC_RedBlackTree_BLACK);
            grandparent.set_color(COMiC_RedBlackTree_RED);

            node = grandparent;
        } while ((parent = node.get_parent()) != nullptr);

        /* https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insert_case_3 */
        return COMiC_SUCCESS;

        POST:
        /* https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insert_case_5 */
        if (parent == grandparent.get_left())
        {
            if (node == parent.get_right())
            {
                _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateLeft(as_parent_wrapper_left_t(grandparent), parent, node);
            }
            else
            { node = parent; }
            parent = grandparent;

            grandparent = parent.get_parent();
            if (grandparent == nullptr)
            { _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateRight(as_parent_wrapper_root_t(self), parent, node); }
            else if (grandparent.get_left() == parent)
            { _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateRight(as_parent_wrapper_left_t(grandparent), parent, node); }
            else
            { _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateRight(as_parent_wrapper_right_t(grandparent), parent, node); }
        }
        else
        {
            if (node == parent.get_left())
            {
                _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateRight(as_parent_wrapper_right_t(grandparent), parent, node);
            }
            else
            { node = parent; }
            parent = grandparent;

            grandparent = parent.get_parent();
            if (grandparent == nullptr)
            { _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateLeft(as_parent_wrapper_root_t(self), parent, node); }
            else if (grandparent.get_left() == parent)
            { _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateLeft(as_parent_wrapper_left_t(grandparent), parent, node); }
            else
            { _COMiC_RedBlackTree::_COMiC_RedBlackTree_RotateLeft(as_parent_wrapper_right_t(grandparent), parent, node); }
        }
        node.set_color(COMiC_RedBlackTree_BLACK);
        parent.set_color(COMiC_RedBlackTree_RED);
        return COMiC_SUCCESS;
    }
}

#endif /* COMiC_RB_TREE_HPP */
