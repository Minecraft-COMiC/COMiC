#ifndef COMIC_NETWORK_H
#define COMIC_NETWORK_H

#include "platform-dependent/platform.h"

#define COMiC_VERSION ("0.0.1")

#include "network_constants.h"
#include "../util/util.h"
#include "main.h"
#include "buffer/buffer.h"

typedef struct COMiC_Network_ClientNetInfo COMiC_Network_ClientNetInfo;
typedef struct COMiC_Network_ServerNetInfo COMiC_Network_ServerNetInfo;

#if defined(WINDOWS)
    #include "platform-dependent/windows/network_windows.h"
#elif defined(LINUX)
    #include "platform-dependent/linux/network_linux.h"
#endif

#include "packet/sender.h"
#include "packet/receiver.h"

#endif //COMIC_NETWORK_H