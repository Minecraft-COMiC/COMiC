cmake_minimum_required(VERSION 3.21)
project(NetworkTest C)

set(CMAKE_C_STANDARD 99)

find_package(OpenSSL REQUIRED)

if (WIN32)
    set(OS_DEPENDENT_LIBRARIES ws2_32 libcrypto)
    set(OS_DEPENDENT_FILES platform-dependent/windows/network_windows.c platform-dependent/windows/network_windows.h)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(OS_DEPENDENT_LIBRARIES ${OPENSSL_LIBRARIES})
    set(OS_DEPENDENT_FILES platform-dependent/linux/network_linux.c platform-dependent/linux/network_linux.h)
endif ()

add_executable(
        NetworkTest

        util.h
        main.c main.h
        buffer/buffer.c buffer/buffer.h
        packet/receiver.c packet/receiver.h
        packet/sender.c packet/sender.h
        juuid/juuid.c juuid/juuid.h
        rsa/rsa.h rsa/rsa.c
        network.h
        ${OS_DEPENDENT_FILES}
        platform-dependent/platform.h
        network_constants.h
)

target_link_libraries(NetworkTest ${OS_DEPENDENT_LIBRARIES})
