cmake_minimum_required(VERSION 3.19)
project(COMiC C)

set(CMAKE_C_STANDARD 99)

#find_package(Java COMPONENTS Runtime)
#find_package(Java COMPONENTS Development)
#find_package(Java COMPONENTS JarSigner)
#include(UseJava)

set(COMiC_VERSION_MAJOR 0)
set(COMiC_VERSION_MINOR 0)
set(COMiC_VERSION_PATCH 0)
set(COMiC_VERSION_LEVEL a)
set(COMiC_VERSION_SERIAL 0)
set(COMiC_VERSION ${COMiC_VERSION_MAJOR}.${COMiC_VERSION_MINOR}.${COMiC_VERSION_PATCH}${COMiC_VERSION_LEVEL}${COMiC_VERSION_SERIAL})
message("Building COMiC ${COMiC_VERSION}")

add_executable(
        _COMiC_version_test

        version_test.c
)
target_include_directories(_COMiC_version_test PRIVATE Include)
target_compile_definitions(
        _COMiC_version_test
        PRIVATE

        CMAKE_COMiC_VERSION_MAJOR=${COMiC_VERSION_MAJOR}
        CMAKE_COMiC_VERSION_MINOR=${COMiC_VERSION_MINOR}
        CMAKE_COMiC_VERSION_PATCH=${COMiC_VERSION_PATCH}
        CMAKE_COMiC_VERSION_LEVEL=${COMiC_VERSION_LEVEL}
        CMAKE_COMiC_VERSION_SERIAL=${COMiC_VERSION_SERIAL}
)


add_custom_target(_COMiC_checks)
add_dependencies(_COMiC_checks _COMiC_version_test)

set(
        COMiC_server_sources

        src/lib.def
        src/lib.c
)

add_library(
        COMiC_lib_limited
        SHARED

        ${COMiC_server_sources}
)
add_dependencies(COMiC_lib_limited _COMiC_checks)
set_target_properties(COMiC_lib_limited PROPERTIES OUTPUT_NAME "comic-l-${COMiC_VERSION}")
target_include_directories(COMiC_lib_limited PUBLIC Include)
target_compile_definitions(COMiC_lib_limited PUBLIC COMiC_LIMITED)

add_library(
        COMiC_lib_fast
        SHARED

        ${COMiC_server_sources}
)
add_dependencies(COMiC_lib_fast _COMiC_checks)
set_target_properties(COMiC_lib_fast PROPERTIES OUTPUT_NAME "comic-${COMiC_VERSION}")
target_include_directories(COMiC_lib_fast PUBLIC Include)
target_compile_definitions(COMiC_lib_fast PUBLIC COMiC_FAST)

add_executable(
        COMiC_exe_limited

        src/main.c
)
target_link_libraries(COMiC_exe_limited PRIVATE COMiC_lib_limited)
set_target_properties(COMiC_exe_limited PROPERTIES OUTPUT_NAME "comic-l-${COMiC_VERSION}")

add_executable(
        COMiC_exe_fast

        src/main.c
)
target_link_libraries(COMiC_exe_fast PRIVATE COMiC_lib_fast)
set_target_properties(COMiC_exe_fast PROPERTIES OUTPUT_NAME "comic-${COMiC_VERSION}")


if (COMiC_LIMITED)
    install(
            TARGETS COMiC_lib_limited COMiC_exe_limited
    )
else ()
    install(
            TARGETS COMiC_lib_fast COMiC_exe_fast
    )
endif ()

#
#add_jar(
#        COMiC_jar
#        main.java
#)
##target_link_libraries(COMiC_jar PRIVATE COMiC_code)
#set_target_properties(COMiC_jar PROPERTIES OUTPUT_NAME comic)


#install(
#        TARGETS COMiC_exe
#)
#install_jar(COMiC_jar DESTINATION java)