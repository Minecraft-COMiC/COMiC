#ifndef COMiC_Core_ARENA_HPP
# define COMiC_Core_ARENA_HPP

# include "types.h"
# include "arena.h"
# include "os.h"

template<COMiC_USize elem_size>
static constexpr inline COMiC_USize _COMiC_Arena_Buffer_CalcSize(
        COMiC_In  COMiC_USize capacity
) noexcept
{ return sizeof(struct _COMiC_Arena_BufferHead) + _COMiC_Arena_GetOccupancyInfoUnitsSize(capacity) + (sizeof(_COMiC_Arena_NodeHead) + elem_size) * capacity; }


template<COMiC_USize elem_size>
static inline COMiC_USize _COMiC_Arena_Buffer_CalcOptimalCapacity(
        COMiC_InOut COMiC_USize *capacity
) noexcept
{
    COMiC_USize pages_count = COMiC_OS_GetNativeMemoryPageCount(_COMiC_Arena_Buffer_CalcSize<elem_size>(*capacity));
    while (pages_count >= COMiC_OS_GetNativeMemoryPageCount(_COMiC_Arena_Buffer_CalcSize<elem_size>(++(*capacity))))
    {}
    return COMiC_OS_GetNativeMemoryPageCount(_COMiC_Arena_Buffer_CalcSize<elem_size>(--(*capacity)));
}

class COMiC_Arena_GrowStrategy_Double
{
public:
    static constexpr inline COMiC_USize extend_capacity(
            COMiC_In COMiC_USize previous_size
    ) noexcept
    {
        if (previous_size == 0)
        { return 1; }
        if (previous_size > ((~((COMiC_USize) 1)) >> 1))
        { return ~((COMiC_USize) 0); }
        else
        { return previous_size << 1; }
    }
};

template<
        COMiC_USize elem_size,
        class grow_strategy = COMiC_Arena_GrowStrategy_Double
>
static inline COMiC_IfError COMiC_Arena_Alloc(
        COMiC_InOut COMiC_Arena *self,
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer
) noexcept
{
    static_assert(elem_size > 0, "Elem size for COMiC_Arena must be greater then 0");

    struct _COMiC_Arena_BufferHead *b;
    COMiC_USize index;
    struct _COMiC_Arena_BufferHead new_buffer;


    for (b = self->last; b != nullptr; b = b->next)
    {
        if (b->used >= b->capacity)
        { continue; }

        if (!_COMiC_Arena_Buffer_FindEmptyCell(b, &index))
        { continue; }

        BUFFER_FOUND:

        _COMiC_Arena_Buffer_SetOccupancy(_COMiC_Arena_Buffer_GetOccupancyInfo(b), index, COMiC_TRUE);
        b->used++;

        void *n = (((COMiC_UIntPtr) _COMiC_Arena_Buffer_GetData(b)) + (sizeof(_COMiC_Arena_NodeHead) + elem_size) * index);
        ((_COMiC_Arena_NodeHead *) n)->owner = b;

        *pointer = (void *) (((COMiC_UIntPtr) n) + sizeof(_COMiC_Arena_NodeHead));

        return COMiC_SUCCESS;
    }

    new_buffer.capacity = self->last == nullptr ? 0 : self->last->capacity;
    new_buffer.used = 0;
    new_buffer.next = self->last;
    new_buffer.prev = nullptr;

    if (COMiC_NativeAllocOnPages(
            error,
            &(self->last),
            _COMiC_Arena_Buffer_CalcOptimalCapacity<elem_size>(&(new_buffer.capacity))
    ))
    { return COMiC_ERROR; }

    *(self->last) = new_buffer;

    index = new_buffer.capacity - 1;
    b = self->last;

    goto BUFFER_FOUND;
}

template<COMiC_USize elem_size>
static inline void COMiC_Arena_DeAlloc(
        COMiC_InOut COMiC_Arena *self,
        COMiC_In void *pointer
) noexcept
{
    COMiC_UnusedArg(self);
    static_assert(elem_size > 0, "Elem size for COMiC_Arena must be greater then 0");

    struct _COMiC_Arena_BufferHead *b = ((_COMiC_Arena_NodeHead *) (((COMiC_UIntPtr) pointer) - sizeof(_COMiC_Arena_NodeHead)))->owner;
    COMiC_USize index = (((COMiC_UIntPtr) pointer) - ((COMiC_UIntPtr) _COMiC_Arena_Buffer_GetData(b))) / (sizeof(_COMiC_Arena_NodeHead) + elem_size);

    _COMiC_Arena_Buffer_SetOccupancy(_COMiC_Arena_Buffer_GetOccupancyInfo(b), index, COMiC_FALSE);
}


#endif /* COMiC_Core_ARENA_HPP */
