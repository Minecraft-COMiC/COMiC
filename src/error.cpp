#include <COMiC/core.hpp>

template<
        COMiC_USize LS1,
        COMiC_USize LS2
>
struct COMiC_ErrorHeap
{
    static const COMiC_USize level1_elem_size = LS1;
    static const COMiC_USize level2_elem_size = LS2;

    COMiC_Arena level1;
    COMiC_Arena level2;
    COMiC_DoublyLinkedList level3;


    constexpr inline COMiC_ErrorHeap() noexcept
    {
        COMiC_Arena_Constructor(&(this->level1));
        COMiC_Arena_Constructor(&(this->level2));
        COMiC_DoublyLinkedList_Constructor(&(this->level3));
    }
};

static COMiC_ErrorHeap<256 + sizeof(COMiC_USize), 1024 + sizeof(COMiC_USize)> COMiC_ErrorHeap_Instance;

static inline COMiC_IfError _COMiC_ErrorHeap_Alloc(
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
) noexcept
{
    if (size <= COMiC_ErrorHeap_Instance.level1_elem_size)
    {
        return COMiC_Arena_Alloc<COMiC_ErrorHeap_Instance.level1_elem_size>(
                &(COMiC_ErrorHeap_Instance.level1),
                error,
                pointer
        );
    }
    else if (size <= COMiC_ErrorHeap_Instance.level2_elem_size)
    {
        return COMiC_Arena_Alloc<COMiC_ErrorHeap_Instance.level2_elem_size>(
                &(COMiC_ErrorHeap_Instance.level2),
                error,
                pointer
        );
    }
    else
    {
        if (COMiC_NativeAllocOnPages(
                error,
                pointer,
                size + sizeof(COMiC_DoublyLinkedList_Node)
        ))
        { return COMiC_ERROR; }

        COMiC_DoublyLinkedList_Link(
                &(COMiC_ErrorHeap_Instance.level3),
                (COMiC_DoublyLinkedList_Node *) pointer
        );
        *pointer = (void *) (((COMiC_UIntPtr) pointer) + sizeof(COMiC_DoublyLinkedList_Node));

        return COMiC_SUCCESS;
    }
}

COMiC_IfError COMiC_ErrorHeap_Alloc(
        COMiC_Out COMiC_Error *error,
        COMiC_Out void **pointer,
        COMiC_In COMiC_USize size
) noexcept
{
    if (_COMiC_ErrorHeap_Alloc(error, pointer, size + sizeof(COMiC_USize)))
    { return COMiC_ERROR; }

    *(COMiC_USize *) *pointer = size;
    *pointer = (void *) (((COMiC_UIntPtr) *pointer) + sizeof(COMiC_USize));

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_ErrorHeap_DeAlloc(
        COMiC_Out COMiC_Error *error,
        COMiC_In void *pointer
) noexcept
{
    pointer = (void *) (((COMiC_UIntPtr) pointer) - sizeof(COMiC_USize));
    COMiC_USize size = *(COMiC_USize *) pointer;

    if (size <= COMiC_ErrorHeap_Instance.level1_elem_size)
    {
        COMiC_Arena_DeAlloc<COMiC_ErrorHeap_Instance.level1_elem_size>(
                &(COMiC_ErrorHeap_Instance.level1),
                pointer
        );
    }
    else if (size <= COMiC_ErrorHeap_Instance.level2_elem_size)
    {
        COMiC_Arena_DeAlloc<COMiC_ErrorHeap_Instance.level2_elem_size>(
                &(COMiC_ErrorHeap_Instance.level2),
                pointer
        );
    }
    else
    {
        pointer = (void *) (((COMiC_UIntPtr) pointer) - sizeof(COMiC_DoublyLinkedList_Node));
        COMiC_DoublyLinkedList_UnLink(
                &(COMiC_ErrorHeap_Instance.level3),
                (COMiC_DoublyLinkedList_Node *) pointer
        );

        if (COMiC_NativeDeAllocOnPages(error, pointer))
        { return COMiC_ERROR; }

        return COMiC_SUCCESS;
    }
}

