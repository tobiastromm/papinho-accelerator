/* Existing portable core contracts, deliberately no Win32 SDK include path. */
#include "papacc/types.h"
#include "papacc/version.h"
#include "capability.h"
#if defined(_WINDOWS_) || defined(_NCRYPT_H_) || defined(__NCRYPT_H__)
#error Native Windows headers leaked into portable contracts
#endif
typedef char core_u32_width[(sizeof(PAPACC_U32)==4)?1:-1];
int main(void){return PAPACC_RESULT_OK;}