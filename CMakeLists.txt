cmake_minimum_required(VERSION 3.19)
project(COMiC C)

set(CMAKE_C_STANDARD 90)

find_package(Java COMPONENTS Runtime)
#find_package(Java COMPONENTS Development)
find_package(Java COMPONENTS JarSigner)

include(UseJava)

add_library(
        COMiC_code
        STATIC

        main.c
)

add_executable(
        COMiC_exe

        main.c
)
target_link_libraries(COMiC_exe PRIVATE COMiC_code)
set_target_properties(COMiC_exe PROPERTIES OUTPUT_NAME comic)


add_jar(
        COMiC_jar
        main.java
)
#target_link_libraries(COMiC_jar PRIVATE COMiC_code)
set_target_properties(COMiC_jar PROPERTIES OUTPUT_NAME comic)

install(
        TARGETS COMiC_exe
)
install_jar(COMiC_jar DESTINATION java)