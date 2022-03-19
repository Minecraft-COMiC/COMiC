#ifndef COMiC_ERROR_H
# define COMiC_ERROR_H

# include <COMiC/types.h>

# ifdef __cplusplus
extern "C" {
#endif

typedef enum COMiC_ErrNo
{
    COMiC_ErrNo_NoError = 0,
    COMiC_ErrNo_External = 1,
} COMiC_ErrNo;

typedef struct COMiC_Error
{
    COMiC_ErrNo err_no;
    COMiC_Optional(NULL) char *message;
    COMiC_Optional(NULL) void *external_data;
} COMiC_Error;

# ifdef __cplusplus
};
#endif

#endif /* COMiC_ERROR_H */
