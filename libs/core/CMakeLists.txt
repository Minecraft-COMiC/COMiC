add_library(
        __COMiC_Core
        STATIC

        src/application.c
)

target_link_libraries(__COMiC_Core PUBLIC COMiC::OS)
target_include_directories(__COMiC_Core PUBLIC include)

add_library(
        COMiC::Core
        ALIAS
        __COMiC_Core
)