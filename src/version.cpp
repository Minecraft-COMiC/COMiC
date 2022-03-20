#include <COMiC.h>

static_assert(
        (COMiC_Version_ReleaseLevel == 0xA) ||
        (COMiC_Version_ReleaseLevel == 0xB) ||
        (COMiC_Version_ReleaseLevel == 0xC) ||
        (COMiC_Version_ReleaseLevel == 0xF),
        "Invalid value of macro 'COMiC_Version_ReleaseLevel'"
);