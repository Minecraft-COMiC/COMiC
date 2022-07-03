#include <COMiC/core.h>

#include "registry.h"

COMiC_IfError COMiC_Test_Registry(
        COMiC_In COMiC_TestContext *context,
        COMiC_Out COMiC_Error *error
) noexcept
{
    COMiC_Test_REGISTRY_ADD_SUBREGISTRY(Core, DoublyLinkedList);
    return COMiC_SUCCESS;
}

COMiC_Test_SETUP_MAIN()