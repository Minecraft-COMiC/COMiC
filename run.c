#include <stdio.h>

#include <COMiC/core.h>

int main()
{
    COMiC_Application app;
    COMiC_Error err;
    char *const some_string;

    COMiC_Error_Init(&err);
    if (COMiC_Application_Init(&app, &err, 0))
    {
        COMiC_Error_Release(&err);
        return 1;
    }

    if (COMiC_Alloc(
            &(app.heap),
            &err,
            (void **) &some_string,
            500
    ))
    {

        COMiC_Error_Release(&err);
        return 3;
    }

    memset(some_string, '#', 499);
    some_string[499] = 0;
    puts(some_string);


    if (COMiC_Free(
            &(app.heap),
            &err,
            (void **) &some_string
    ))
    {

        COMiC_Error_Release(&err);
        return 4;
    }

    if (COMiC_Application_Finalize(&app, &err))
    {
        COMiC_Error_Release(&err);
        return 2;
    }

    return 0;
}