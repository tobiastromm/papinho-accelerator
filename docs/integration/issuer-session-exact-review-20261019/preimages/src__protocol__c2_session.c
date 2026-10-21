#include "c2_session.h"
#include <string.h>
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
PAPACC_BOOL papacc_server_launcher_c2_session_current(const PAPACC_C2_SESSION *);
#endif
static const PAPACC_CAPABILITY_KEY tls_key={2U,1U},c2_key={3U,1U};
static int owner_valid(const PAPACC_C2_SESSION*s,PAPACC_U32 owner){return s!=NULL&&s->initialized==PAPACC_TRUE&&s->owner==owner;}
static int both(const PAPACC_C2_SESSION*s){return s->grant_count==2U&&papacc_capability_key_equal(&s->grants[0],&tls_key)&&papacc_capability_key_equal(&s->grants[1],&c2_key);}
PAPACC_RESULT papacc_c2_session_init(PAPACC_C2_SESSION*s,PAPACC_U32 owner,PAPACC_U64 incarnation,PAPACC_U64 principal,PAPACC_U64 epoch,PAPACC_U8 assurance,PAPACC_BOOL tls,PAPACC_BOOL c3,PAPACC_BOOL lab){
 if(s==NULL||owner==0U||incarnation==0U||epoch==0U||(assurance!=1U&&assurance!=2U))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(s->initialized==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 if((assurance==1U&&principal==0U)||(assurance==2U&&(principal!=0U||lab!=PAPACC_TRUE)))return PAPACC_RESULT_INVALID_STATE;
 memset(s,0,sizeof(*s));s->owner=owner;s->incarnation=incarnation;s->principal=principal;s->policy_epoch=epoch;s->assurance=assurance;s->tls_allowed=tls==PAPACC_TRUE?PAPACC_TRUE:PAPACC_FALSE;s->c3_supported=c3==PAPACC_TRUE?PAPACC_TRUE:PAPACC_FALSE;s->legacy_lab=lab;s->initialized=PAPACC_TRUE;s->state=PAPACC_C2_SESSION_CONFIGURED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_server_negotiate(PAPACC_C2_SESSION*s,PAPACC_U32 owner,const PAPACC_U8*in,PAPACC_SIZE n,PAPACC_U8*out,PAPACC_SIZE cap,PAPACC_SIZE*written){
 PAPACC_CAPABILITY_KEY sk[2],pk[2],ek[2];PAPACC_CAPABILITY_SET supported=PAPACC_CAPABILITY_SET_INITIALIZER,preferred=PAPACC_CAPABILITY_SET_INITIALIZER,effective=PAPACC_CAPABILITY_SET_INITIALIZER;PAPACC_RESULT result;
 if(written!=NULL)*written=0;
 if(out==NULL||written==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!owner_valid(s,owner)||s->state!=PAPACC_C2_SESSION_CONFIGURED)return PAPACC_RESULT_INVALID_STATE;
 (void)papacc_capability_set_init(&supported,sk,2);(void)papacc_capability_set_init(&preferred,pk,2);(void)papacc_capability_set_init(&effective,ek,2);
 result=papacc_capability_negotiate_decode(in,n,&supported,&preferred);
 if(result!=0){s->state=PAPACC_C2_SESSION_REVOKED;return result;}
 /* Dedicated C2 Session's exact immutable offered/preference composition, not key growth. */
 if(supported.count!=2U||preferred.count!=2U||!papacc_capability_key_equal(sk,&tls_key)||!papacc_capability_key_equal(sk+1,&c2_key)||!papacc_capability_key_equal(pk,&tls_key)||!papacc_capability_key_equal(pk+1,&c2_key)){s->state=PAPACC_C2_SESSION_REVOKED;return PAPACC_RESULT_PROTOCOL_ERROR;}
 if(s->tls_allowed){(void)papacc_capability_set_insert(&effective,&tls_key);if(s->c3_supported)(void)papacc_capability_set_insert(&effective,&c2_key);}
 result=papacc_capability_negotiate_result_encode(&effective,out,cap,written);
 if(result!=0)return result; /* No result/grant commitment before atomic encode success. */
 s->grant_count=effective.count;if(effective.count)memcpy(s->grants,ek,effective.count*sizeof(ek[0]));s->state=PAPACC_C2_SESSION_RESULT_PENDING;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_server_result_written(PAPACC_C2_SESSION*s,PAPACC_U32 owner,PAPACC_BOOL complete,PAPACC_U64 epoch){
 if(!owner_valid(s,owner)||s->state!=PAPACC_C2_SESSION_RESULT_PENDING)return PAPACC_RESULT_INVALID_STATE;
 if(epoch!=s->policy_epoch){s->state=PAPACC_C2_SESSION_REVOKED;return PAPACC_RESULT_INVALID_STATE;}
 if(complete!=PAPACC_TRUE)return PAPACC_RESULT_OK;
 s->state=both(s)?PAPACC_C2_SESSION_ELIGIBLE:PAPACC_C2_SESSION_UNAVAILABLE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_browser_result_received(PAPACC_C2_SESSION*s,PAPACC_U32 owner,const PAPACC_U8*in,PAPACC_SIZE n,PAPACC_U64 epoch){
 PAPACC_CAPABILITY_KEY ek[2];PAPACC_CAPABILITY_SET effective=PAPACC_CAPABILITY_SET_INITIALIZER;PAPACC_RESULT result;PAPACC_SIZE i;
 if(!owner_valid(s,owner)||s->state!=PAPACC_C2_SESSION_CONFIGURED)return PAPACC_RESULT_INVALID_STATE;
 if(epoch!=s->policy_epoch){s->state=PAPACC_C2_SESSION_REVOKED;return PAPACC_RESULT_INVALID_STATE;}
 (void)papacc_capability_set_init(&effective,ek,2);result=papacc_capability_negotiate_result_decode(in,n,&effective);
 if(result!=0){s->state=PAPACC_C2_SESSION_REVOKED;return result;}
 for(i=0;i<effective.count;++i)if(!papacc_capability_key_equal(ek+i,&tls_key)&&!papacc_capability_key_equal(ek+i,&c2_key)){s->state=PAPACC_C2_SESSION_REVOKED;return PAPACC_RESULT_PROTOCOL_ERROR;}
 s->grant_count=effective.count;if(effective.count)memcpy(s->grants,ek,effective.count*sizeof(ek[0]));s->state=both(s)&&s->tls_allowed&&s->c3_supported?PAPACC_C2_SESSION_ELIGIBLE:PAPACC_C2_SESSION_UNAVAILABLE;return PAPACC_RESULT_OK;
}
PAPACC_BOOL papacc_c2_session_authorized(const PAPACC_C2_SESSION*s,PAPACC_U32 owner,PAPACC_U64 incarnation,PAPACC_U64 principal,PAPACC_U64 epoch){
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 if(!papacc_server_launcher_c2_session_current(s))return PAPACC_FALSE;
#endif
 return owner_valid(s,owner)&&s->state==PAPACC_C2_SESSION_ELIGIBLE&&both(s)&&s->incarnation==incarnation&&s->principal==principal&&s->policy_epoch==epoch&&s->tls_allowed&&s->c3_supported&&(s->assurance==1U||(s->assurance==2U&&s->legacy_lab))?PAPACC_TRUE:PAPACC_FALSE;
}
PAPACC_RESULT papacc_c2_session_revoke(PAPACC_C2_SESSION*s,PAPACC_U32 owner,PAPACC_BOOL retained){
 if(!owner_valid(s,owner))return PAPACC_RESULT_INVALID_STATE;
 if(s->state==PAPACC_C2_SESSION_RETAINED)return PAPACC_RESULT_OK;
 s->state=retained==PAPACC_TRUE?PAPACC_C2_SESSION_RETAINED:PAPACC_C2_SESSION_REVOKED;return PAPACC_RESULT_OK;
}
