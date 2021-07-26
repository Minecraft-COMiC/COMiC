#include <COMiC.h>

#if CMAKE_COMiC_VERSION_MAJOR != COMiC_VERSION_MAJOR
# error major version not compatible
#endif
#if CMAKE_COMiC_VERSION_MINOR != COMiC_VERSION_MINOR
# error minor version not compatible
#endif
#if CMAKE_COMiC_VERSION_PATCH != COMiC_VERSION_PATCH
# error patch version not compatible
#endif
#if 0 && CMAKE_COMiC_VERSION_LEVEL != COMiC_VERSION_LEVEL
# error patch level not compatible
#endif
#if CMAKE_COMiC_VERSION_SERIAL != COMiC_VERSION_SERIAL
# error patch level serial not compatible
#endif


int main(void)
{
    return 0;
}