add_library(
        util
        STATIC

        src/error.c
        src/version.cpp
)

target_include_directories(util PUBLIC include)

add_library(
        COMiC::Util
        ALIAS
        util
)

install(
        TARGETS util
)
install(
        DIRECTORY ./include
        DESTINATION ./
)