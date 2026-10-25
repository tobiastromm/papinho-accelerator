#include "config_root8_time_source_key.h"
#include "../issuer/strict_ed25519.h"
#include <string.h>
PC_ROOT8_RESULT pc_root8_time_source_key_data(void *ctx,unsigned kind,
 const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header)
{
 int result;
 (void)ctx;
 if(kind!=9U)return PC_ROOT8_UNSUPPORTED;
 if(!p||n!=384U||p!=row||!header)return PC_ROOT8_INVALID;
 if(memcmp(header,"PACROOT8",8)||header[8]!=4U||header[9]||
    memcmp(p,"PACTSR01",8)||p[8]!=1U||p[9]||p[10]!=128U||p[11]!=1U)
  return PC_ROOT8_INVALID;
 result=papacc_time_source_ed25519_check(p+88U,32U);
 if(result==PAPACC_ED25519_VALID)return PC_ROOT8_OK;
 if(result==PAPACC_ED25519_INVALID)return PC_ROOT8_INVALID;
 if(result==PAPACC_ED25519_BACKEND_UNAVAILABLE)return PC_ROOT8_UNSUPPORTED;
 return PC_ROOT8_INCOMPLETE;
}
