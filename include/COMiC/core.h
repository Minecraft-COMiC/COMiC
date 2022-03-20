#ifndef COMiC_CORE_H
# define COMiC_CORE_H

# include <COMiC/types.h>
# include <COMiC/error.h>
# include <COMiC/os.h>

# ifdef __cplusplus
extern "C" {
# endif


typedef struct COMiC_Application
{
    COMiC_OS_Heap heap;
} COMiC_Application;


COMiC_Constructor
COMiC_IfError COMiC_Application_Init(
        COMiC_Out COMiC_Application *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error,
        COMiC_In COMiC_Optional(0) COMiC_USize max_heap_size
);


COMiC_Destructor
COMiC_IfError COMiC_Application_Finalize(
        COMiC_Out COMiC_Application *self,
        COMiC_Out COMiC_Optional(NULL) COMiC_Error *error
);

# ifdef __cplusplus
};
# endif

#endif /* COMiC_CORE_H */
