#ifndef COMiC_Core_TEST_H
# define COMiC_Core_TEST_H

# include "types.h"
# include "error.h"
# include "doubly_linked_list.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef enum COMiC_Test_Kind
{
    COMiC_Test_Kind_CORRECTNESS = 1,
    COMiC_Test_Kind_PERFOMANCE = 2,
} COMiC_Test_Kind;

typedef struct COMiC_TestCaseContext COMiC_TestCaseContext;

typedef COMiC_IfError (*COMiC_TestCaseProc)(COMiC_TestCaseContext *);

struct _COMiC_TestCase
{
    struct _COMiC_TestCase *next;
    COMiC_Test_Kind kind;
    char const *name;
    COMiC_TestCaseProc proc;
};

COMiC_IfError COMiC_Test_DisableTestMode(
        COMiC_Out COMiC_Error *error
) noexcept;

struct COMiC_TestCaseContext
{
    struct _COMiC_TestCase *meta;
};

typedef struct COMiC_TestContext
{
    struct _COMiC_TestCase *first;
    struct _COMiC_TestCase *last;
    COMiC_USize test_cases_count;
    COMiC_Arena test_cases_buffer;
} COMiC_TestContext;

COMiC_IfError COMiC_Test_Registry(
        COMiC_In COMiC_TestContext *context,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_TestContext_Constructor(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_TestContext_Destructor(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error
) noexcept;

COMiC_IfError COMiC_TestContext_AddTest(
        COMiC_Out COMiC_TestContext *self,
        COMiC_Out COMiC_Error *error,
        COMiC_In COMiC_Test_Kind kind,
        COMiC_In char const *name,
        COMiC_In COMiC_TestCaseProc proc
) noexcept;

# define COMiC_Test_REGISTRY_FUNCTION_NAME(LIBRARY_NAME) COMiC_Test_Registry

# define COMiC_Test_REGISTRY_FUNCTION_HEAD(LIBRARY_NAME)                       \
    COMiC_IfError COMiC_Test_REGISTRY_FUNCTION_NAME(LIBRARY_NAME)(             \
        COMiC_In COMiC_TestContext *context,                                   \
        COMiC_Out COMiC_Error *error                                           \
    ) noexcept

# define COMiC_Test_REGISTRY_SUBFUNCTION_NAME(LIBRARY_NAME, SUBMODULE_NAME) COMiC_Test_ ## SUBMODULE_NAME ## _Registry

# define COMiC_Test_REGISTRY_SUBFUNCTION_HEAD(LIBRARY_NAME, SUBMODULE_NAME)                    \
    COMiC_IfError COMiC_Test_REGISTRY_SUBFUNCTION_NAME(LIBRARY_NAME, SUBMODULE_NAME)(          \
        COMiC_In COMiC_TestContext *context,                                                   \
        COMiC_Out COMiC_Error *error                                                           \
    ) noexcept

# define COMiC_Test_REGISTRY_ADD_SUBREGISTRY(LIBRARY_NAME, SUBMODULE_NAME)                     \
    if (COMiC_Test_REGISTRY_SUBFUNCTION_NAME(LIBRARY_NAME, SUBMODULE_NAME)(context, error))    \
    { return COMiC_ERROR; }

# define COMiC_Test_REGISTRY_ADD_TEST(KIND, NAME, PROC)                                        \
    if (COMiC_TestContext_AddTest(context, error, (COMiC_Test_Kind_ ## KIND), (NAME), (PROC))) \
    { return COMiC_ERROR; }


# ifdef __cplusplus
#  define COMiC_Test_SETUP_MAIN()                                              \
    extern "C" {                                                               \
        int main(int argc, char *argv[])                                       \
        {                                                                      \
            int COMiC_Test_Main(int, char*[]);                                 \
            return COMiC_Test_Main(argc, argv);                                \
        }                                                                      \
    }
# else
#  define COMiC_Test_SETUP_MAIN()                                              \
    int main(int argc, char *argv[])                                           \
    {                                                                          \
        int COMiC_Test_Main(int, char*[]);                                     \
        return COMiC_Test_Main(argc, argv);                                    \
    }
# endif

COMiC_IfError COMiC_Test_FailedAssertion(
        COMiC_Out COMiC_Error *error,
        COMiC_In char *format,
        COMiC_In ...
) noexcept;

COMiC_IfError COMiC_Test_FailedWithError(
        COMiC_Out COMiC_Error *outer_error,
        COMiC_In COMiC_Error *inner_error,
        COMiC_In COMiC_Optional(NULL) char *format,
        COMiC_In ...
) noexcept;

void COMiC_Test_ResetDuration(void) noexcept;

long double COMiC_Test_GetDuration(void) noexcept;


# ifdef __cplusplus
}
# endif

#endif /* COMiC_Core_TEST_H */
