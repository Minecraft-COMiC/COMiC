#ifndef COMiC_Core_DOUBLY_LINKED_LIST_HPP
# define COMiC_Core_DOUBLY_LINKED_LIST_HPP

# include "types.hpp"

namespace COMiC::Core
{
    class DoublyLinkedList
    {
    public:
        class Node
        {
        private:
            COMiC::Core::DoublyLinkedList::Node *next = nullptr;
            COMiC::Core::DoublyLinkedList::Node *prev = nullptr;
        public:
            constexpr inline void linkTo(
                    COMiC_InOut COMiC::Core::DoublyLinkedList *const list
            ) noexcept
            {
                if (list->last == nullptr)
                {
                    list->first = list->last = this;
                    this->prev  = this->next = nullptr;
                }
                else
                {
                    list->last->next = this;
                    this->prev       = list->last;
                    this->next       = nullptr;
                    list->last       = this;
                }
            }

            constexpr inline void unlinkFrom(
                    COMiC_InOut COMiC::Core::DoublyLinkedList *const list
            ) noexcept
            {
                if (this->prev != nullptr)
                { this->prev->next = this->next; }
                else
                { list->first = this->next; }

                if (this->next != nullptr)
                { this->next->prev = this->prev; }
                else
                { list->last = this->prev; }
            }

            COMiC_NODISCARD
            constexpr inline COMiC::Core::DoublyLinkedList::Node *getNext() const
            {
                return this->next;
            }

            COMiC_NODISCARD
            constexpr inline COMiC::Core::DoublyLinkedList::Node *getPrev() const
            {
                return this->prev;
            }

            constexpr inline void linkInsteadOf(
                    COMiC_InOut COMiC::Core::DoublyLinkedList *const list,
                    COMiC_In COMiC::Core::DoublyLinkedList::Node *const destination
            ) noexcept
            {
                this->prev = destination->prev;

                if (destination->prev == NULL)
                    list->first = this;
                else
                    this->prev->next = this;

                this->next = destination->next;

                if (destination->next == NULL)
                    list->last = this;
                else
                    this->next->prev = this;
            }
        };

    private:
        COMiC::Core::DoublyLinkedList::Node *first = nullptr;
        COMiC::Core::DoublyLinkedList::Node *last  = nullptr;

    public:
        constexpr inline DoublyLinkedList() noexcept = default;

        inline ~DoublyLinkedList() noexcept = default;

        COMiC_NODISCARD
        constexpr inline COMiC::Bool isEmpty() const noexcept
        {
            return this->first == nullptr;
        }


        class Iterator
        {
        private:
            COMiC::Core::DoublyLinkedList::Node *pos;
        public:
            constexpr inline explicit Iterator(
                    COMiC_In COMiC::Core::DoublyLinkedList *const list
            ) noexcept: pos(list->first)
            {}

            COMiC_NODISCARD
            constexpr inline COMiC::IteratorState next(
                    COMiC_Out COMiC::Core::DoublyLinkedList::Node **const node
            ) noexcept
            {
                if (this->pos == nullptr)
                    return COMiC::ENDED;

                this->pos = (*node = this->pos)->getNext();
                return COMiC::VALUE_FOUND;
            }
        };


        class ReversedIterator
        {
        private:
            COMiC::Core::DoublyLinkedList::Node *pos;
        public:
            constexpr inline explicit ReversedIterator(
                    COMiC_In COMiC::Core::DoublyLinkedList *const list
            ) noexcept: pos(list->last)
            {}

            COMiC_NODISCARD
            constexpr inline COMiC::IteratorState next(
                    COMiC_Out COMiC::Core::DoublyLinkedList::Node **const node
            ) noexcept
            {
                if (this->pos == nullptr)
                    return COMiC::ENDED;

                this->pos = (*node = this->pos)->getPrev();
                return COMiC::VALUE_FOUND;
            }
        };

        class FastFreeIterator
        {
        private:
            COMiC::Core::DoublyLinkedList::Iterator iterator;
        public:
            constexpr inline explicit FastFreeIterator(
                    COMiC_In COMiC::Core::DoublyLinkedList *const list
            ) noexcept: iterator(list)
            {}

            COMiC_NODISCARD
            constexpr inline COMiC::IteratorState next(
                    COMiC_Out COMiC::Core::DoublyLinkedList::Node **const node
            ) noexcept
            {
                return this->iterator.next(node);
            }
        };
    };
}

#endif /* COMiC_Core_DOUBLY_LINKED_LIST_HPP */
