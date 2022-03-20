#ifndef COMiC_TYPES_H
# define COMiC_TYPES_H

# ifdef NULL
#  undef NULL
# endif
# define NULL ((void *)0)

typedef int COMiC_Bool;
# define COMiC_FALSE (0)
# define COMiC_TRUE (1)

typedef COMiC_Bool COMiC_IfError;
# define COMiC_SUCCESS COMiC_FALSE
# define COMiC_ERROR COMiC_TRUE

typedef size_t COMiC_USize;

# define COMiC_Constructor
# define COMiC_Destructor

# define COMiC_Optional(VALUE)

# define COMiC_In
# define COMiC_Out

#endif /* COMiC_TYPES_H */
