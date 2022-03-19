#ifndef COMiC_TYPES_H
# define COMiC_TYPES_H

# ifndef NULL
#  define NULL ((void *)0)
# endif

typedef int COMiC_Bool;
# define COMiC_FALSE (0)
# define COMiC_TRUE (1)

typedef COMiC_Bool COMiC_IfError;
# define COMiC_SUCCESS COMiC_FALSE
# define COMiC_ERROR COMiC_TRUE

typedef size_t COMiC_USize;

# define _COMiC_Unlock(TYPE, MEMBER) (*(TYPE*)&(MEMBER))

# define COMiC_Optional(VALUE)

# define COMiC_In
# define COMiC_Out

#endif /* COMiC_TYPES_H */
