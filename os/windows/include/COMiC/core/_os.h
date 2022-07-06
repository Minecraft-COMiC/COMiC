#ifndef COMiC_Core__OS_H
# define COMiC_Core__OS_H

# include <windows.h>
#include <COMiC/core.hpp>
#include <cstdio>

struct COMiC_OS_Lock
{
    CRITICAL_SECTION critical_section;
};

typedef struct COMiC_OS_Windows_ErrorExtraInfo
{
    DWORD error_code;
} COMiC_OS_Windows_ErrorExtraInfo;

//extern COMiC_ErrorClass const *const COMiC_OS_Windows_SystemErrorClass;

namespace COMiC::Network::OS
{
    struct Socket
    {
        SOCKET socket;
    };

    struct InetAddr
    {
        struct sockaddr_in address;
    };
}

namespace COMiC::Core::OS
{
    static inline void RandomBytes(COMiC::Byte *out, COMiC_In COMiC::USize len)
    {
        HCRYPTPROV p;

        if (CryptAcquireContext(&p, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)
            fprintf(stderr, "RandomBytes(): CryptAcquireContext failed.");

        if (CryptGenRandom(p, len, (BYTE *) out) == FALSE)
            fprintf(stderr, "RandomBytes(): CryptGenRandom failed.");

        CryptReleaseContext(p, 0);
    }
}

#endif /* COMiC_Core__OS_H */
