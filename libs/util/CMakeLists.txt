add_library(
        __COMiC_Util
        INTERFACE
)

target_include_directories(__COMiC_Util INTERFACE include)

add_library(
        COMiC::Util
        ALIAS
        __COMiC_Util
)