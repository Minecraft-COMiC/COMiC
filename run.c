#include <stdio.h>

#include <COMiC.h>

int main()
{
    COMiC_Heap heap;
    COMiC_Error err;
    struct COMiC_SingleBuffer buffer;
    int i;
    COMiC_Byte some_data = '*';
    void *pointer_list[1000];

    COMiC_Error_Init(&err);
    if (COMiC_Heap_Init(&heap, &err, 0))
    {
        COMiC_Error_Release(&err);
        return 1;
    }

    if (COMiC_SingleBuffer_Init(&buffer, &err, &heap, 48))
    {
        COMiC_Error_Release(&err);
        return 3;
    }

    for (i = 0; i < 1000; i++)
    {
        if (COMiC_SingleBuffer_Push(&buffer, &err, &some_data, sizeof(COMiC_Byte), pointer_list + i))
        {
            COMiC_Error_Release(&err);
            return 0x1000 | i;
        }
    }
    for (i = 200; i < 600; i++)
    {
        if (COMiC_SingleBuffer_Pop(&buffer, &err, pointer_list + i))
        {
            COMiC_Error_Release(&err);
            return 0x2000 | i;
        }
    }


    if (COMiC_SingleBuffer_Clear(&buffer, &err))
    {
        COMiC_Error_Release(&err);
        return 5;
    }

    for (i = 0; i < 200; i++)
    {
        if (COMiC_SingleBuffer_Pop(&buffer, &err, pointer_list + i))
        {
            COMiC_Error_Release(&err);
            return 0x3000 | i;
        }
    }

    for (i = 600; i < 1000; i++)
    {
        if (COMiC_SingleBuffer_Pop(&buffer, &err, pointer_list + i))
        {
            COMiC_Error_Release(&err);
            return 0x4000 | i;
        }
    }


    if (COMiC_SingleBuffer_Finalize(&buffer, &err))
    {
        COMiC_Error_Release(&err);
        return 4;
    }

    if (COMiC_Heap_Finalize(&heap, &err))
    {
        COMiC_Error_Release(&err);
        return 2;
    }

    return 0;
}