/* Existing private receipt extension; deterministic public-field projection.
 * No allocation, I/O, callbacks, native effects or operational authority. */
#include "server_c2_owner_win32.h"
#include <string.h>
static PAPACC_U64 pair_u64(const PAPACC_U8*p){unsigned i;PAPACC_U64 n=0;for(i=0;i<8U;++i)n=(n<<8)|p[i];return n;}
static PAPACC_U32 pair_u32(const PAPACC_U8*p){unsigned i;PAPACC_U32 n=0;for(i=0;i<4U;++i)n=(n<<8)|p[i];return n;}
PAPACC_RESULT papacc_c2_pair_open_receipt(const PAPACC_U8*p,PAPACC_SIZE n,PAPACC_U64 sequence,PAPACC_C2_PAIR_OPEN_RECEIPT*out){
 PAPACC_C2_PAIR_OPEN_RECEIPT f;ULONG_PTR a=(ULONG_PTR)p,b=(ULONG_PTR)out;
 if(!p||!out||n!=108U||!sequence)return PAPACC_RESULT_INVALID_ARGUMENT;
 if((b>=a&&b-a<n)||(b<a&&a-b<sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 /* C2 accepted OPEN_RESULT: only the granted path has a nonzero tuple.
  * Bytes44..107 are secret tokens and are NEVER read or copied here. */
 if(p[40]||p[41]||p[42]!=1U||p[43])return PAPACC_RESULT_INVALID_STATE;
 memset(&f,0,sizeof(f));f.version=1U;f.present=1U;f.sequence=sequence;
 f.request=pair_u64(p);f.operation=pair_u64(p+8U);f.generation=pair_u64(p+16U);f.epoch=pair_u64(p+24U);
 f.attempt_generation=pair_u32(p+32U);f.resolution_generation=pair_u32(p+36U);
 if(!f.request||!f.operation||!f.generation||!f.epoch||!f.attempt_generation||!f.resolution_generation)return PAPACC_RESULT_INVALID_STATE;
 *out=f;return PAPACC_RESULT_OK;
}