add_executable(
        __COMiC_Core_Test

        src/main.c
        src/doubly_linked_list.cpp src/registry.h)

target_link_libraries(__COMiC_Core_Test PRIVATE COMiC::Core)
# target_link_options(__COMiC_Core_Test PRIVATE /ENTRY:COMiC_Test_Main)

add_test(NAME COMiC::Core COMMAND __COMiC_Core_Test)