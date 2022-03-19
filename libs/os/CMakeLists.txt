set(COMiC_COMMON_OS_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


install(
        DIRECTORY ./include
        DESTINATION ./
)

if (NOT EXISTS COMiC::OS)
    if (${WIN32})
        add_subdirectory(windows)
    else ()
        message(FATAL_ERROR "Unsupported operating system")
    endif ()
endif ()