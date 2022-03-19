add_library(
        core
        STATIC

        src/application.c
)

target_link_libraries(core PUBLIC COMiC::OS)
target_include_directories(core PUBLIC include)

add_library(
        COMiC::Core
        ALIAS
        core
)


install(
        TARGETS core
)
install(
        DIRECTORY ./include
        DESTINATION ./
)