#ifndef COMiC_VERSION_H
# define COMiC_VERSION_H

# define COMiC_Version_Major (0u)
# define COMiC_Version_Minor (0u)
# define COMiC_Version_Micro (0u)
# define COMiC_Version_ReleaseLevel (0xAu)
# define COMiC_Version_Serial (0u)
# define COMiC_Version ((COMiC_Version_Major << 24u) | (COMiC_Version_Minor << 16u) | (COMiC_Version_Micro << 8u) | (COMiC_Version_ReleaseLevel << 4u) | COMiC_Version_Serial)

#endif /* COMiC_VERSION_H */
