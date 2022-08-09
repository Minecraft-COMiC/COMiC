#include <iostream>
#include <COMiC/compression.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/network.hpp>

#include <WinSock2.h>
#include <Windows.h>

using namespace COMiC;

void ProcessStdin()
{
    INPUT_RECORD record;
    DWORD numRead;
    if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &numRead))
    {
        // hmm handle this error somehow...
        return;
    }

    if (record.EventType != KEY_EVENT)
    {
        // don't care about other console events
        return;
    }

    if (!record.Event.KeyEvent.bKeyDown)
    {
        // really only care about keydown
        return;
    }

    // if you're setup for ASCII, process this:
    //record.Event.KeyEvent.uChar.AsciiChar

}

int main()
{
    Crypto::init();
    Compression::init();

    Network::NETSERVICE.start();

    OS::setCtrlCHandler();

    while (COMiC::alive)
    {
        switch (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), INFINITE))
        {
            case WAIT_OBJECT_0:
                ProcessStdin();
                break;
            default:
                break;
        }
    }

    Network::NETSERVICE.end();

    std::system("pause");
    return 0;
}