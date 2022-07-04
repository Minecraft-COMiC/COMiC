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


static COMiC_IfError COMiC_TestCaseContext_Constructor(
        COMiC_Out COMiC_TestCaseContext *self,
        COMiC_In struct _COMiC_TestCase *meta
) noexcept
{
    self->meta = meta;
    return COMiC_SUCCESS;
}

static COMiC_IfError COMiC_TestCaseContext_Destructor(
        COMiC_In COMiC_TestCaseContext *self
) noexcept
{
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_TestContext_RunTests(
        COMiC_In COMiC_TestContext *context,
        COMiC_Out COMiC_TestResults *results,
        COMiC_Out COMiC_Error *error
) noexcept
{
    struct _COMiC_TestCase *current_test;
    struct COMiC_TestCaseContext current_test_context;
    struct COMiC_TestCaseResult *current_test_result;

    COMiC_Arena_Constructor(&(results->results_buffer));

    for (current_test = context->first; current_test != NULL; current_test = current_test->next)
    {
        if (COMiC_Arena_Alloc<sizeof(struct COMiC_TestCaseResult)>(&(results->results_buffer), error, &current_test_result))
        { return COMiC_ERROR; }

        if (COMiC_TestCaseContext_Constructor(&current_test_context, current_test))
        { return COMiC_ERROR; }

        if (current_test->proc(&current_test_context))
        { return COMiC_ERROR; }

        if (COMiC_TestCaseContext_Destructor(&current_test_context))
        { return COMiC_ERROR; }
    }

    return COMiC_SUCCESS;
}
