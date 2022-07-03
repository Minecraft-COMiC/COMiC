#include <COMiC/core.h>

#include "registry.h"

COMiC_IfError hello_world(COMiC_TestCaseContext *context) {
    return COMiC_ERROR;
};

COMiC_Test_REGISTRY_SUBFUNCTION_HEAD(Core, DoublyLinkedList)
{

    COMiC_Test_REGISTRY_ADD_TEST(CORRECTNESS, "hello world", hello_world);
    return COMiC_SUCCESS;
}