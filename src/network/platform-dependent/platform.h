#ifndef COMIC_PLATFORM_H
#define COMIC_PLATFORM_H

#if defined(__linux__) || defined(linux) || defined(__linux)
    #define LINUX
#elif defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__CYGWIN__)
    #define WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #error "No MAC allowed!" //#define MAC_OS
#else
    #error "Unsupported platform!"
#endif

#endif //COMIC_PLATFORM_H
