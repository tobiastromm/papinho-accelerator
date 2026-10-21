#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>
#include "c2_tokens_win32.h"
#include <string.h>
int papacc_c2_compare(const void *,const void *,size_t);
#ifdef PAPACC_C2_TEST_ONLY_FAULTS
static PAPACC_C2_TEST_RNG test_rng;
void papacc_c2_tokens_test_rng(PAPACC_C2_TEST_RNG rng){test_rng=rng;}
#endif
static PAPACC_RESULT random_bytes(PAPACC_U8*p){
#ifdef PAPACC_C2_TEST_ONLY_FAULTS
 if(test_rng!=NULL)return test_rng(p,32);
#endif
 return BCryptGenRandom(NULL,p,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG)==0?PAPACC_RESULT_OK:PAPACC_RESULT_INTERNAL_ERROR;
}
static int nz(const PAPACC_U8*p,PAPACC_SIZE n){unsigned value=0;while(n--!=0U)value|=*p++;return value!=0U;}
static PAPACC_U64 get64(const PAPACC_U8*p){PAPACC_U64 v=0;unsigned i;for(i=0;i<8;++i)v=(v<<8)|p[i];return v;}
static int allowed(const PAPACC_C2_TOKENS*t,PAPACC_U32 owner){return t!=NULL&&t->generated&&t->owner==owner&&!t->revoked&&(PAPACC_U64)GetTickCount64()<t->expires_ms&&papacc_c2_session_authorized(t->session,owner,t->incarnation,t->principal,t->policy_epoch);}
PAPACC_RESULT papacc_c2_tokens_generate(PAPACC_C2_TOKENS*t,const PAPACC_C2_SESSION*s,PAPACC_U32 owner,const PAPACC_U8*identity,const PAPACC_C2_TOKENS*live,PAPACC_SIZE count){
 PAPACC_C2_TOKENS candidate;PAPACC_SIZE i;unsigned attempts=0,role,j;int duplicate;
 if(t==NULL||s==NULL||identity==NULL||(count!=0U&&live==NULL)||count>2U)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(t->generated||!papacc_c2_session_authorized(s,owner,s->incarnation,s->principal,s->policy_epoch)||!nz(identity,8)||!nz(identity+8,8)||!nz(identity+16,8)||!nz(identity+24,4)||!nz(identity+28,4)||get64(identity+16)!=s->policy_epoch)return PAPACC_RESULT_INVALID_STATE;
 memset(&candidate,0,sizeof(candidate));candidate.session=s;candidate.owner=owner;memcpy(candidate.identity,identity,32);candidate.incarnation=s->incarnation;candidate.principal=s->principal;candidate.policy_epoch=s->policy_epoch;candidate.expires_ms=(PAPACC_U64)GetTickCount64()+5000U;
 for(role=0;role<2;++role){
  for(;;){
   if(attempts==4U){SecureZeroMemory(&candidate,sizeof(candidate));return PAPACC_RESULT_INTERNAL_ERROR;}
   ++attempts;if(random_bytes(candidate.bytes[role])!=0){SecureZeroMemory(&candidate,sizeof(candidate));return PAPACC_RESULT_INTERNAL_ERROR;}
   duplicate=!nz(candidate.bytes[role],32);
   if(role&&papacc_c2_compare(candidate.bytes[role],candidate.bytes[0],32)==0)duplicate=1;
   for(i=0;i<count;++i)if(live[i].generated)for(j=0;j<2;++j)if(papacc_c2_compare(candidate.bytes[role],live[i].bytes[j],32)==0)duplicate=1;
   if(!duplicate)break;
   SecureZeroMemory(candidate.bytes[role],32);
  }
 }
 candidate.generated=PAPACC_TRUE;*t=candidate;SecureZeroMemory(&candidate,sizeof(candidate));return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_tokens_grant_written(PAPACC_C2_TOKENS*t,PAPACC_U32 owner){if(!allowed(t,owner)||t->grant_written)return PAPACC_RESULT_INVALID_STATE;t->grant_written=PAPACC_TRUE;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_c2_tokens_claim(PAPACC_C2_TOKENS*t,PAPACC_U32 owner,PAPACC_U64 incarnation,PAPACC_U64 principal,PAPACC_U64 channel,const PAPACC_U8*p,PAPACC_SIZE n){
 unsigned role;PAPACC_RESULT result;
 if(!allowed(t,owner)||!t->grant_written||channel==0U||incarnation!=t->incarnation||principal!=t->principal)return PAPACC_RESULT_INVALID_STATE;
 result=papacc_c2_validate_payload(18U,p,n,PAPACC_C2_ROLE_CHANNEL,PAPACC_C2_BROWSER);if(result!=0)return result;
 if(memcmp(p,t->identity,32))return PAPACC_RESULT_INVALID_STATE;
 role=p[32]-1U;if(t->claimed[role]||t->channels[1U-role]==channel)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_c2_compare(p+36,t->bytes[role],32)!=0)return PAPACC_RESULT_INVALID_STATE;
 t->claimed[role]=PAPACC_TRUE;t->channels[role]=channel;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_tokens_ack_written(PAPACC_C2_TOKENS*t,PAPACC_U32 owner,unsigned role,PAPACC_U64 channel){
 if(!allowed(t,owner)||role<1U||role>2U||!t->claimed[role-1U]||t->ack_written[role-1U]||t->channels[role-1U]!=channel)return PAPACC_RESULT_INVALID_STATE;
 t->ack_written[role-1U]=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_tokens_borrow(PAPACC_C2_TOKENS*t,PAPACC_U32 owner){if(!allowed(t,owner)||t->borrowers>=16U)return PAPACC_RESULT_INVALID_STATE;++t->borrowers;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_c2_tokens_quiescent(PAPACC_C2_TOKENS*t,PAPACC_U32 owner){
 if(t==NULL||!t->generated||t->owner!=owner||t->borrowers==0U)return PAPACC_RESULT_INVALID_STATE;
 --t->borrowers;if(t->revoked&&t->borrowers==0U)SecureZeroMemory(t->bytes,sizeof(t->bytes));return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_tokens_revoke(PAPACC_C2_TOKENS*t,PAPACC_U32 owner,PAPACC_BOOL retained){
 if(t==NULL||!t->generated||t->owner!=owner)return PAPACC_RESULT_INVALID_STATE;
 t->revoked=PAPACC_TRUE;if(retained)t->retained=PAPACC_TRUE;
 if(t->borrowers==0U)SecureZeroMemory(t->bytes,sizeof(t->bytes));return PAPACC_RESULT_OK;
}
