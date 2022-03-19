#include <COMiC/core.h>

int main()
{
    COMiC_Application app;
    COMiC_Error err;

    COMiC_Error_Init(&err);
    if (COMiC_Application_Init(&app, &err, 0))
    {
        COMiC_Error_Release(&err);
        return 1;
    }
    if (COMiC_Application_Finalize(&app, &err))
    {
        COMiC_Error_Release(&err);
        return 2;
    }
    return 0;
}