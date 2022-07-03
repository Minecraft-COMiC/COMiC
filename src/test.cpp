#include <COMiC/core.hpp>

COMiC_IfError COMiC_TestContext_AddTest(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Test_Kind kind,
        COMiC_In char const *name,
        COMiC_In COMiC_TestCaseProc proc
) noexcept
{
    _COMiC_TestCase *test_case;

    if (COMiC_Arena_Alloc<sizeof(_COMiC_TestCase)>(&(self->test_cases_buffer), error, &test_case))
    { return COMiC_ERROR; }

    test_case->next = NULL;
    if (self->first == NULL)
    { self->first = self->last = test_case; }
    else
    { self->last = self->last->next = test_case; }

    test_case->kind = kind;
    test_case->name = name;
    test_case->proc = proc;

    self->test_cases_count++;

    return COMiC_SUCCESS;
}