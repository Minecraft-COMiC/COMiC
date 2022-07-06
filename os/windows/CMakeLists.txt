add_library(
        __COMiC_OS_Windows
        OBJECT

        src/network/network.hpp
        src/network/network.cpp
)

target_link_libraries(__COMiC_OS_Windows PUBLIC COMiC::Headers)
target_include_directories(__COMiC_OS_Windows PUBLIC ./include)

add_library(
        COMiC::OS
        ALIAS
        __COMiC_OS_Windows
)

install(
        DIRECTORY ./include
        DESTINATION ./
)