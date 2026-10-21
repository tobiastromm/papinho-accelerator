#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>
#include "enrollment.h"
EP_RESULT ep_pair_generate_windows(EP_PAIR *p,const PAPACC_U8 client[32],PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now,const EP_PAIR *live,PAPACC_SIZE count){
 PAPACC_U8 random[55];EP_RESULT result;
 if(!p||!client||p->initialized||p->stopped||!connection||!generation||count>2U||(count&&!live))return EP_INVALID;
 if(BCryptGenRandom(NULL,random,sizeof(random),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){SecureZeroMemory(random,sizeof(random));return EP_DENIED;}
 result=ep_pair_prepare_data(p,random,client,connection,generation,now,live,count);SecureZeroMemory(random,sizeof(random));return result;
}
