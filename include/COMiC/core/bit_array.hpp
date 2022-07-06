#ifndef COMiC_Core_BIT_ARRAY_HPP
# define COMiC_Core_BIT_ARRAY_HPP


# include "types.hpp"

namespace COMiC::Core
{
    class EmbedBitArray
    {
    private:
        class Cell
        {
        private:
            COMiC::FastestUInt value = 0;
        public:
            constexpr inline Cell() noexcept = default;

            inline ~Cell() noexcept = default;

            constexpr inline COMiC::Bool operator[](
                    COMiC_In COMiC::USize pos
            ) const noexcept
            {
                return this->value & (((COMiC::FastestUInt) 1) << pos);
            }

            constexpr inline void set(
                    COMiC_In COMiC::USize pos
            ) noexcept
            {
                this->value |= ((COMiC::FastestUInt) 1) << pos;
            }

            constexpr inline void clear(
                    COMiC_In COMiC::USize pos
            ) noexcept
            {
                this->value &= ~(((COMiC::FastestUInt) 1) << pos);
            }

            COMiC_NODISCARD
            constexpr inline COMiC::Bool isFull() const noexcept
            {
                return this->value == ~((COMiC::FastestUInt) 0);
            }

            constexpr inline COMiC::Bool find(
                    COMiC_Out COMiC::USize *const index
            ) const noexcept
            {
                COMiC::USize       bit  = COMiC::Core::EmbedBitArray::BITS_PER_CELL - 1;
                COMiC::FastestUInt mask = ((COMiC::FastestUInt) 1) << bit;

                do
                {
                    if ((this->value & mask) == 0)
                    {
                        *index = bit;
                        return true;
                    }
                } while (--bit, (mask >>= 1) != 0);
                return false;
            }
        };

        static constexpr const COMiC::USize BITS_PER_CELL = sizeof(COMiC::Core::EmbedBitArray::Cell) * COMiC::BYTE_SIZE;

        static_assert(COMiC::Core::EmbedBitArray::BITS_PER_CELL > 0, "Bit array is degenerate");

        COMiC::Core::EmbedBitArray::Cell array[0];

        static constexpr inline COMiC::USize length2Cells(
                COMiC_In COMiC::USize length
        ) noexcept
        {
            COMiC::USize cells = length / COMiC::Core::EmbedBitArray::BITS_PER_CELL;
            COMiC::USize bits  = length * COMiC::Core::EmbedBitArray::BITS_PER_CELL;
            return cells + (bits > 0 ? 1 : 0);
        }

    public:
        static constexpr inline COMiC::USize sizeOfLength(
                COMiC_In COMiC::USize length
        ) noexcept
        {
            return COMiC::Core::EmbedBitArray::length2Cells(length) * sizeof(COMiC::Core::EmbedBitArray::Cell);
        }

        inline explicit EmbedBitArray(
                COMiC_In COMiC::USize length
        ) noexcept: array()
        {
            COMiC::USize i = COMiC::Core::EmbedBitArray::length2Cells(length);
            if (i == 0)
                return;

            while (i-- > 0)
                new(&(this->array[i])) COMiC::Core::EmbedBitArray::Cell;

            COMiC::USize bit = length % COMiC::Core::EmbedBitArray::BITS_PER_CELL;
            if (bit == 0)
                return;

            bit = COMiC::Core::EmbedBitArray::BITS_PER_CELL - bit;
            i   = COMiC::Core::EmbedBitArray::length2Cells(length) - 1;
            while (bit-- > 0)
                this->array[i].set(bit);
        }

        inline ~EmbedBitArray() noexcept = default;

        constexpr inline COMiC::Bool operator[](
                COMiC_In COMiC::USize index
        ) const noexcept
        {
            return this->array[index / COMiC::Core::EmbedBitArray::BITS_PER_CELL][index % COMiC::Core::EmbedBitArray::BITS_PER_CELL];
        }

        constexpr inline void set(
                COMiC_In COMiC::USize index
        ) noexcept
        {
            this->array[index / COMiC::Core::EmbedBitArray::BITS_PER_CELL].set(index % COMiC::Core::EmbedBitArray::BITS_PER_CELL);
        }

        constexpr inline void clear(
                COMiC_In COMiC::USize index
        ) noexcept
        {
            this->array[index / COMiC::Core::EmbedBitArray::BITS_PER_CELL].clear(index % COMiC::Core::EmbedBitArray::BITS_PER_CELL);
        }

        COMiC_NODISCARD
        constexpr inline COMiC::Bool find(
                COMiC_In COMiC::USize const length,
                COMiC_Out COMiC::USize *const index
        ) const noexcept
        {
            COMiC::USize i = COMiC::Core::EmbedBitArray::length2Cells(length);
            while (i-- > 0)
            {
                if (this->array[i].isFull())
                    continue;

                if (this->array[i].find(index))
                {
                    *index += i * COMiC::Core::EmbedBitArray::BITS_PER_CELL;
                    return true;
                }
            }
            return false;
        }

        template<COMiC::Bool filled>
        class Iterator
        {
        private:
            COMiC::Core::EmbedBitArray::Cell *cell;
            COMiC::USize                     pos;
            COMiC::USize                     bit;
        public:
            constexpr inline Iterator(
                    COMiC_In COMiC::Core::EmbedBitArray *array,
                    COMiC_In COMiC::USize length
            ) noexcept: pos(length), cell(&(array->array[length / COMiC::Core::EmbedBitArray::BITS_PER_CELL])), bit(length % COMiC::Core::EmbedBitArray::BITS_PER_CELL)
            {}

            constexpr inline COMiC::IteratorState next(COMiC::USize *index) noexcept
            {
                while (true)
                {
                    if (this->bit == 0)
                    {
                        if (this->pos == 0)
                            return COMiC::ENDED;

                        this->bit = COMiC::Core::EmbedBitArray::BITS_PER_CELL;
                        this->cell--;
                    }

                    this->pos--;

                    if ((*this->cell)[--(this->bit)] == filled)
                    {
                        *index = this->pos;
                        return COMiC::VALUE_FOUND;
                    }
                }
            }

            constexpr inline void set() noexcept
            {
                this->cell->set(this->bit);
            }

            constexpr inline void clear() noexcept
            {
                this->cell->clear(this->bit);
            }
        };

        using FilledIterator = COMiC::Core::EmbedBitArray::Iterator<true>;
        using FreeIterator = COMiC::Core::EmbedBitArray::Iterator<false>;
    };
}


#endif /* COMiC_Core_BIT_ARRAY_HPP */
