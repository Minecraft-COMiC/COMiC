add_executable(
        __COMiC_Core_Test

        ./src/main.c
        ./src/registry.h
        ./src/doubly_linked_list.cpp
)

target_link_libraries(__COMiC_Core_Test PRIVATE COMiC::Core COMiC::TestRuntime)
# target_link_options(__COMiC_Core_Test PRIVATE /ENTRY:COMiC_Test_Main)

add_test(NAME COMiC::Core COMMAND __COMiC_Core_Test)