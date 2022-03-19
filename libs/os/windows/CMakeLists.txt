add_library(
        os
        STATIC

        src/error.h
        src/error.c
        src/heap.c
)

target_link_libraries(os PUBLIC COMiC::Util)
target_include_directories(os PUBLIC include)
target_include_directories(os PUBLIC ${COMiC_COMMON_OS_HEADER_DIR})

add_library(
        COMiC::OS
        ALIAS
        os
)

install(
        TARGETS os
)
install(
        DIRECTORY ./include
        DESTINATION ./
)