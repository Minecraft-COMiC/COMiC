cmake_minimum_required(VERSION 3.19)
project(COMiC C)

set(CMAKE_C_STANDARD 11)

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


# check for compatibility cmake and lib version
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


# root dependency for all targets
add_custom_target(_COMiC_checks)
add_dependencies(_COMiC_checks _COMiC_version_test)

# sources of COMiC implementation
set(
        COMiC_lib_sources

        src/lib.def
        src/lib.c
)

# limited implementation
add_library(
        COMiC_lib_limited
        SHARED

        ${COMiC_lib_sources}
)
add_dependencies(COMiC_lib_limited _COMiC_checks)
set_target_properties(COMiC_lib_limited PROPERTIES OUTPUT_NAME "comic-l-${COMiC_VERSION}")
target_include_directories(COMiC_lib_limited PUBLIC Include)
target_compile_definitions(COMiC_lib_limited PUBLIC COMiC_LIMITED)


# fast (default) implementation
add_library(
        COMiC_lib_fast
        SHARED

        ${COMiC_lib_sources}
)
add_dependencies(COMiC_lib_fast _COMiC_checks)
set_target_properties(COMiC_lib_fast PROPERTIES OUTPUT_NAME "comic-${COMiC_VERSION}")
target_include_directories(COMiC_lib_fast PUBLIC Include)
target_compile_definitions(COMiC_lib_fast PUBLIC COMiC_FAST)


# limited server entry point
add_executable(
        COMiC_server_exe_limited

        server/main.c
)
target_link_libraries(COMiC_server_exe_limited PRIVATE COMiC_lib_limited)
set_target_properties(COMiC_server_exe_limited PROPERTIES OUTPUT_NAME "comic-sl-${COMiC_VERSION}")


# fast (default) server entry point
add_executable(
        COMiC_server_exe_fast

        server/main.c
)
target_link_libraries(COMiC_server_exe_fast PRIVATE COMiC_lib_fast)
set_target_properties(COMiC_server_exe_fast PROPERTIES OUTPUT_NAME "comic-s-${COMiC_VERSION}")


# limited client entry point
add_executable(
        COMiC_client_exe_limited

        server/main.c
)
target_link_libraries(COMiC_client_exe_limited PRIVATE COMiC_lib_limited)
set_target_properties(COMiC_client_exe_limited PROPERTIES OUTPUT_NAME "comic-cl-${COMiC_VERSION}")


# fast (default) client entry point
add_executable(
        COMiC_client_exe_fast

        server/main.c
)
target_link_libraries(COMiC_client_exe_fast PRIVATE COMiC_lib_fast)
set_target_properties(COMiC_client_exe_fast PROPERTIES OUTPUT_NAME "comic-c-${COMiC_VERSION}")


# final dependency for limited package
add_custom_target(COMiC_limited)
add_dependencies(COMiC_limited COMiC_server_exe_limited COMiC_client_exe_limited)


# final dependency for fast (default) package
add_custom_target(COMiC_fast)
add_dependencies(COMiC_fast COMiC_server_exe_fast COMiC_client_exe_fast)


# final dependency for all artifacts
add_custom_target(_COMiC_full)
add_dependencies(_COMiC_full COMiC_fast COMiC_limited)

if (COMiC_LIMITED)
    install(
            TARGETS COMiC_lib_limited COMiC_server_exe_limited COMiC_client_exe_limited
    )
else ()
    install(
            TARGETS COMiC_lib_fast COMiC_server_exe_fast COMiC_client_exe_fast
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