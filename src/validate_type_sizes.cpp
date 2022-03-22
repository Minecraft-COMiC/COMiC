#include <COMiC.h>

#define checkEq(TYPE, SIZE) static_assert(sizeof(TYPE) == (SIZE), "Size of type '" #TYPE "' must be '" #SIZE "'")
#define checkGe(TYPE, SIZE) static_assert(sizeof(TYPE) >= (SIZE), "Size of type '" #TYPE "' must be greater or equal to '" #SIZE "'")

/* int & float types */
checkEq(COMiC_Byte, 1);
checkEq(COMiC_Word, 2);
checkEq(COMiC_DWord, 4);
checkEq(COMiC_QWord, 8);
checkEq(COMiC_UIntPtr, sizeof(void *));

checkGe(COMiC_Int8, 1);
checkGe(COMiC_UInt8, 1);
checkGe(COMiC_Int16, 2);
checkGe(COMiC_UInt16, 2);
checkGe(COMiC_Int32, 4);
checkGe(COMiC_UInt32, 4);
checkGe(COMiC_Int64, 8);
checkGe(COMiC_UInt64, 8);

checkGe(COMiC_Float, 4);
checkGe(COMiC_Double, 8);

/* array structures */
checkEq(COMiC_Chunk_Block, 4 * sizeof(COMiC_UIntPtr));
checkEq(struct COMiC_Chunk_Pie, 16 * 16 * sizeof(COMiC_Chunk_Block));