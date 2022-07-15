add_library(
        __COMiC_OS_Linux
        OBJECT

        ./network/network.hpp
        ./network/network.cpp
)

find_package(CURL REQUIRED)
target_link_libraries(__COMiC_OS_Linux PUBLIC COMiC::Headers curl)
target_include_directories(__COMiC_OS_Linux PUBLIC ./include)

add_library(
        COMiC::OS
        ALIAS
        __COMiC_OS_Linux
)

install(
        DIRECTORY ./include
        DESTINATION ./
)