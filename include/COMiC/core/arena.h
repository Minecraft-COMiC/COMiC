#ifndef COMiC_Core_ARENA_H
# define COMiC_Core_ARENA_H

# include "types.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_Arena
{
    struct _COMiC_Arena_BufferHead *last;
} COMiC_Arena;

struct _COMiC_Arena_BufferHead
{
    struct _COMiC_Arena_BufferHead *prev;
    struct _COMiC_Arena_BufferHead *next;
    COMiC_USize used;
    COMiC_USize capacity;
};

struct _COMiC_Arena_NodeHead
{
    struct _COMiC_Arena_BufferHead *owner;
};

static inline COMiC_FastestNumber *_COMiC_Arena_Buffer_GetOccupancyInfo(
        COMiC_In struct _COMiC_Arena_BufferHead *buffer
) noexcept
{ return (COMiC_FastestNumber *) (((COMiC_UIntPtr) buffer) + sizeof(struct _COMiC_Arena_BufferHead)); }

static constexpr inline COMiC_USize _COMiC_Arena_GetOccupancyInfoUnitsSize(
        COMiC_In COMiC_USize capacity
) noexcept
{ return (capacity + (sizeof(COMiC_FastestNumber) * 8) - 1) / (sizeof(COMiC_FastestNumber) * 8); }

static constexpr inline COMiC_USize _COMiC_Arena_Buffer_GetOccupancyInfoUnitsSize(
        COMiC_In struct _COMiC_Arena_BufferHead *buffer
) noexcept
{ return _COMiC_Arena_GetOccupancyInfoUnitsSize(buffer->capacity); }

static inline void *_COMiC_Arena_Buffer_GetData(
        COMiC_In struct _COMiC_Arena_BufferHead *buffer
) noexcept
{ return (void *) (((COMiC_UIntPtr) (_COMiC_Arena_Buffer_GetOccupancyInfo(buffer))) + _COMiC_Arena_Buffer_GetOccupancyInfoUnitsSize(buffer) * sizeof(COMiC_FastestNumber)); }

static constexpr inline COMiC_Bool _COMiC_Arena_Buffer_GetOccupancy(
        COMiC_In COMiC_FastestNumber *occupancy_info,
        COMiC_In COMiC_USize pos
) noexcept
{ return ((((COMiC_FastestNumber *) occupancy_info)[pos / (sizeof(COMiC_FastestNumber) * 8)]) & (((COMiC_FastestNumber) 1) << (pos % (sizeof(COMiC_FastestNumber) * 8)))) == 0 ? COMiC_FALSE : COMiC_TRUE; }

static constexpr inline void _COMiC_Arena_Buffer_SetOccupancy(
        COMiC_In COMiC_FastestNumber *occupancy_info,
        COMiC_In COMiC_USize pos,
        COMiC_In COMiC_Bool value
) noexcept
{
    if (value)
    { (((COMiC_FastestNumber *) occupancy_info)[pos / (sizeof(COMiC_FastestNumber) * 8)]) |= (((COMiC_FastestNumber) 1) << (pos % (sizeof(COMiC_FastestNumber) * 8))); }
    else
    { (((COMiC_FastestNumber *) occupancy_info)[pos / (sizeof(COMiC_FastestNumber) * 8)]) &= ~(((COMiC_FastestNumber) 1) << (pos % (sizeof(COMiC_FastestNumber) * 8))); }
}

static inline COMiC_Bool _COMiC_Arena_Buffer_FindEmptyCell(
        COMiC_In struct _COMiC_Arena_BufferHead *self,
        COMiC_Out COMiC_USize *index
) noexcept {
    COMiC_USize pos = _COMiC_Arena_Buffer_GetOccupancyInfoUnitsSize(self);
    COMiC_FastestNumber *info = _COMiC_Arena_Buffer_GetOccupancyInfo(self) + pos;
    COMiC_FastestNumber mask = ((COMiC_FastestNumber) 1) << ((self->capacity - 1) % (sizeof(COMiC_FastestNumber) * 8));
    *index = self->capacity;

    while (pos-- != 0)
    {
        info--;
        for (; mask != 0; mask >>= 1)
        {
            if ((((*index)--) & mask) == 0)
            { return COMiC_TRUE; }
        }
        mask = ((COMiC_FastestNumber) 1) << (sizeof(COMiC_FastestNumber) * 8 - 1);
    }
    return COMiC_FALSE;
}

static constexpr inline void COMiC_Arena_Constructor(
        COMiC_Out COMiC_Arena *self
) noexcept
{
    self->last = NULL;
}

static inline COMiC_IfError COMiC_Arena_Destructor(
        COMiC_In COMiC_Arena *self,
        COMiC_Out COMiC_Error *error
) noexcept
{
    struct _COMiC_Arena_BufferHead *p;
    while ((p = self->last) != NULL)
    {
        self->last = self->last->next;
        if (COMiC_NativeDeAllocOnPages(error, (void *) p))
        { return COMiC_ERROR; }
    }

    return COMiC_SUCCESS;
}


typedef struct COMiC_Arena_FastFreeIterator
{
    struct _COMiC_Arena_BufferHead *buffer;
    COMiC_USize index;
} COMiC_Arena_FastFreeIterator;

static constexpr inline void COMiC_Arena_FastFreeIterator_Constructor(
        COMiC_Out COMiC_Arena_FastFreeIterator *self,
        COMiC_In COMiC_Arena *arena
) noexcept
{
    self->buffer = arena->last;
    self->index = self->buffer->capacity;
}

static constexpr inline void COMiC_Arena_FastFreeIterator_Destructor(
        COMiC_In COMiC_Arena_FastFreeIterator *self
) noexcept {
    COMiC_UnusedArg(self);
}


# ifdef __cplusplus
}
# endif

#endif /* COMiC_Core_ARENA_H */
