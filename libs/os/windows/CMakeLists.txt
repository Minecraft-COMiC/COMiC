add_library(
        __COMiC_OS_Windows
        STATIC

        src/error.h
        src/error.c
        src/heap.c
)

target_link_libraries(__COMiC_OS_Windows PUBLIC COMiC::Util)
target_include_directories(__COMiC_OS_Windows PUBLIC include)
target_include_directories(__COMiC_OS_Windows PUBLIC ${COMiC_COMMON_OS_HEADER_DIR})

add_library(
        COMiC::OS
        ALIAS
        __COMiC_OS_Windows
)