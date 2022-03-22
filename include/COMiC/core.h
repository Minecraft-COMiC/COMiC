#ifndef COMiC_CORE_H
# define COMiC_CORE_H

# ifdef __cplusplus
extern "C" {
# endif

typedef struct COMiC_Application COMiC_Application;

# ifdef __cplusplus
};
# endif

# include <COMiC/types.h>
# include <COMiC/error.h>
# include <COMiC/os.h>


# ifdef __cplusplus
extern "C" {
# endif


struct COMiC_Application
{
    COMiC_Heap heap;
};


COMiC_Constructor
COMiC_IfError COMiC_Application_Init(
        COMiC_Out COMiC_Application *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_heap_size
);


COMiC_Destructor
COMiC_IfError COMiC_Application_Finalize(
        COMiC_In COMiC_Application *self,
        COMiC_Out COMiC_Error *error
);

struct COMiC_SingleBuffer
{
    COMiC_Heap *const heap;
    COMiC_USize capacity;
    COMiC_USize free;
    const COMiC_USize page_size;
    void *buffer;
    void *top;
};


COMiC_Constructor
COMiC_IfError COMiC_SingleBuffer_Init(
        COMiC_Out struct COMiC_SingleBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Heap *heap,
        COMiC_In COMiC_USize page_size
);


COMiC_Destructor
COMiC_IfError COMiC_SingleBuffer_Finalize(
        COMiC_In struct COMiC_SingleBuffer *self,
        COMiC_Out COMiC_Error *error
);


COMiC_IfError COMiC_SingleBuffer_Push(
        COMiC_In struct COMiC_SingleBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void *data,
        COMiC_In COMiC_USize data_size,
        COMiC_Out void **stored
);

COMiC_IfError COMiC_SingleBuffer_Pop(
        COMiC_In struct COMiC_SingleBuffer *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In void **stored
);


COMiC_IfError COMiC_SingleBuffer_Clear(
        COMiC_In struct COMiC_SingleBuffer *self,
        COMiC_Out COMiC_Error *error
);

# ifdef __cplusplus
};
# endif

#endif /* COMiC_CORE_H */
