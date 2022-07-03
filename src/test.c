#include <stdlib.h>
#include <stdio.h>

#include <COMiC/core.h>


COMiC_IfError COMiC_TestContext_Constructor(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    COMiC_UnusedArg(error);
    self->first = NULL;
    self->last = NULL;
    self->test_cases_count = 0;
    COMiC_Arena_Constructor(&(self->test_cases_buffer));

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_TestContext_Destructor(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error
) noexcept {
    if (COMiC_Arena_Destructor(&(self->test_cases_buffer), error))
    { return COMiC_ERROR; }

    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_TestContext_AddTest(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Test_Kind kind,
        COMiC_In char const *name,
        COMiC_In COMiC_TestCaseProc proc
) noexcept {
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_Test_FailedAssertion(
        COMiC_Out COMiC_Error *error,
        COMiC_In char *format,
        COMiC_In ...
) noexcept
{
    return COMiC_SUCCESS;
}

COMiC_IfError COMiC_Test_FailedWithError(
        COMiC_Out COMiC_Error *outer_error,
        COMiC_In COMiC_Error *inner_error,
        COMiC_In COMiC_Optional(NULL) char *format,
        COMiC_In ...
) noexcept
{
    return COMiC_SUCCESS;
}

void COMiC_Test_ResetDuration(void) noexcept
{}

long double COMiC_Test_GetDuration(void) noexcept
{ return 0.0l; }


int COMiC_Test_Main(int argc, char *argv[])
{
    COMiC_Error error;
    COMiC_TestContext context;

    if (COMiC_TestContext_Constructor(&context, &error))
    {
        fputs("Failed to setup test context:", stderr);
        return COMiC_Error_PrintTopLevel(COMiC_Error_PrintTopLevel_StdLibPuts, stderr, &error);
    }

    if (COMiC_Test_Registry(&context, &error))
    {
        fputs("Failed register tests:", stderr);
        return COMiC_Error_PrintTopLevel(COMiC_Error_PrintTopLevel_StdLibPuts, stderr, &error);
    }


    if (COMiC_TestContext_Destructor(&context, &error))
    {
        fputs("Failed to release test context:", stderr);
        return COMiC_Error_PrintTopLevel(COMiC_Error_PrintTopLevel_StdLibPuts, stderr, &error);
    }

    return EXIT_SUCCESS;
}