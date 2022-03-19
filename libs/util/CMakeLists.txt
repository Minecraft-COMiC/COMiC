add_library(
        __COMiC_Util
        STATIC

        src/error.c
)

target_include_directories(__COMiC_Util PUBLIC include)

add_library(
        COMiC::Util
        ALIAS
        __COMiC_Util
)