#ifndef COMiC_Core__OS_H
# define COMiC_Core__OS_H

#include <cstdio>
#include <Windows.h>
#include <COMiC/core.hpp>
#include <COMiC/network.hpp>

BOOL WINAPI CtrlHandler(DWORD event)
{
    switch (event)
    {
        case CTRL_BREAK_EVENT:
        case CTRL_C_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        case CTRL_CLOSE_EVENT:
            COMiC::alive = false;
            std::cout << "Shutting down..." << std::endl;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

void COMiC::OS::setCtrlCHandler()
{
    SetConsoleCtrlHandler(&CtrlHandler, TRUE);
}

#endif /* COMiC_Core__OS_H */
