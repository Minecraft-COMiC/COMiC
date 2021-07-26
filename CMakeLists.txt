cmake_minimum_required(VERSION 3.19)
project(COMIC C)

set(CMAKE_C_STANDARD 90)

find_package(Java COMPONENTS Runtime)
#find_package(Java COMPONENTS Development)
find_package(Java COMPONENTS JarSigner)

include(UseJava)

add_library(
        COMIC_code
        STATIC

        main.c
)

add_executable(
        COMIC_exe

        main.c
)
target_link_libraries(COMIC_exe PRIVATE COMIC_code)
set_target_properties(COMIC_exe PROPERTIES OUTPUT_NAME comic)


add_jar(
        COMIC_jar
        main.java
)
#target_link_libraries(COMIC_jar PRIVATE COMIC_code)
set_target_properties(COMIC_jar PROPERTIES OUTPUT_NAME comic)

install(
        TARGETS COMIC_exe
)
install_jar(COMIC_jar DESTINATION java)