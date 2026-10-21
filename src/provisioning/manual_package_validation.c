#include "manual_package_validation_internal.h"
#include "shared_structure/m1_internal.h"
#include "../core/ed11_process_profile.h"
#include <stdint.h>
#include <string.h>
static int span(const void *p,size_t n){return p&&n&&n-1<=UINTPTR_MAX-(uintptr_t)p;}
static int overlaps(const void *p,size_t n,const void *q,size_t m){
 uintptr_t a=(uintptr_t)p,b=(uintptr_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);
}
static int stopped(const MP_AUTH_INPUT *i){return i->stopped&&i->stopped(i->stop_context);}
static void wipe(MP_AUTH_OPERATION *o){
 const MP_AUTH_PORT *p=o->internal_port;const MP_AUTH_INPUT *i=o->input;
 p->wipe(o->internal_context,o->key,sizeof(o->key));
 p->wipe(o->internal_context,i->quarantine,i->quarantine_capacity);
 p->wipe(o->internal_context,i->staging,i->staging_capacity);o->staging_size=0;
}
static int arguments(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i){
 const void *p[9];size_t n[9];unsigned a,b,count=7;
 if(!span(o,sizeof(*o))||!span(i,sizeof(*i)))return 0;
 p[0]=o;n[0]=sizeof(*o);p[1]=i;n[1]=sizeof(*i);
 p[2]=i->package.data;n[2]=i->package.size;p[3]=i->password.data;n[3]=i->password.size;
 p[4]=i->quarantine;n[4]=i->quarantine_capacity;p[5]=i->staging;n[5]=i->staging_capacity;p[6]=i->output;n[6]=i->output_capacity;
 if(n[2]>MP_INPUT_MAX||n[3]>128||n[4]>MP_INPUT_MAX||n[5]>MP_INPUT_MAX||n[6]>MP_INPUT_MAX)return 0;
 if(i->pdg){
  if(!span(i->pdg,sizeof(*i->pdg))||i->pdg->root.size>PC_ROOT8_V3_MAX)return 0;
  p[7]=i->pdg;n[7]=sizeof(*i->pdg);p[8]=i->pdg->root.data;n[8]=i->pdg->root.size;count=9;
 }
 for(a=0;a<count;a++){if(!span(p[a],n[a]))return 0;for(b=0;b<a;b++)if(overlaps(p[a],n[a],p[b],n[b]))return 0;}
 return 1;
}
static int budget(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i){
 size_t total=sizeof(*o)+sizeof(*i); /* retained envelope/payload already inside operation */
 size_t n[9];unsigned j,count=7;
 n[0]=i->package.size;n[1]=i->password.size;n[2]=i->quarantine_capacity;
 n[3]=i->staging_capacity;n[4]=i->output_capacity;n[5]=i->other_reserved;n[6]=i->provider_reserved;
 if(i->pdg){n[7]=sizeof(*i->pdg);n[8]=i->pdg->root.size;count=9;}
 for(j=0;j<count;j++){if(n[j]>(size_t)PAPACC_ED11_PROCESS_LIMIT||total>(size_t)PAPACC_ED11_PROCESS_LIMIT-n[j])return 0;total+=n[j];}
 o->managed_estimate=total;return 1;
}
/* Fixed integration bridge; public caller cannot select either binding. */
static int perform(void *owner,SP_M1_STAGE which,int *detail){
 MP_AUTH_OPERATION *o=owner;const MP_AUTH_INPUT *i=o->input;
 const MP_AUTH_PORT *p=o->internal_port;MP_RESULT parsed;int code;
 if(which==MP_AUTH_STRUCTURE){
  parsed=mp_envelope_inspect(i->package.data,i->package.size,&o->envelope,stopped(i));
  *detail=parsed;return parsed==MP_DATA_ONLY?SP_M1_COMPLETE:parsed==MP_STOPPED?SP_M1_STOPPED:SP_M1_INVALID;
 }
 if(which==MP_AUTH_BUDGET){
  *detail=MP_AUTH_LIMIT;
  if(o->envelope.ciphertext.size>MP_ARCHIVE_MAX||i->quarantine_capacity<o->envelope.ciphertext.size||
   i->staging_capacity<o->envelope.ciphertext.size||i->output_capacity<o->envelope.ciphertext.size||!budget(o,i))return SP_M1_LIMIT;
  return SP_M1_COMPLETE;
 }
 if(which==SP_M1_CONTENT_PARSE){
  *detail=MP_AUTH_STEP_INVALID;
  if(!o->staging_size||o->staging_size!=o->envelope.ciphertext.size)return SP_M1_DENIED;
  parsed=mp_payload_inspect(i->staging,o->staging_size,&o->payload,stopped(i));
  if(parsed!=MP_DATA_ONLY){*detail=parsed;return parsed==MP_STOPPED?SP_M1_STOPPED:SP_M1_DENIED;}
  return SP_M1_COMPLETE;
 }
 code=p->step(o->internal_context,which,o,&o->envelope,
  which>=MP_AUTH_CONTENT?&o->payload:NULL);
 *detail=code;
 return code==MP_AUTH_STEP_OK?SP_M1_COMPLETE:code==MP_AUTH_STOPPED?SP_M1_STOPPED:SP_M1_DENIED;
}
static int owner_stopped(void *owner){return stopped(((MP_AUTH_OPERATION *)owner)->input);}
static int owner_quiescent(void *owner){MP_AUTH_OPERATION *o=owner;const MP_AUTH_PORT *p=o->internal_port;return p->quiescent(o->internal_context);}
static void owner_wipe(void *owner){wipe(owner);}
static void owner_publish(void *owner){MP_AUTH_OPERATION *o=owner;memcpy(o->input->output,o->input->staging,o->staging_size);o->output_size=o->staging_size;}
static const SP_M1_BINDING binding={perform,owner_stopped,owner_quiescent,owner_wipe,owner_publish};
static MP_AUTH_RESULT run(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i,const MP_AUTH_PORT *p,void *ctx){
 int result;
 if(!p||!p->step||!p->wipe||!p->quiescent||!arguments(o,i)||o->flow.state)return MP_AUTH_INVALID;
 o->input=i;o->internal_port=p;o->internal_context=ctx;o->output_size=0;
 memset(&o->pdg_data,0,sizeof(o->pdg_data));memset(o->pdg_package_digest,0,sizeof(o->pdg_package_digest));
 result=sp_m1_execute(&o->flow,&binding,o);
 if(result!=SP_M1_COMPLETE)return (MP_AUTH_RESULT)result;
#ifdef MP_PACKAGE_VALIDATION_OFFLINE_TEST
 return MP_AUTH_SYNTHETIC_ONLY;
#else
 return MP_AUTH_VALIDATED; /* native TRUST/source/content remain fail closed */
#endif
}
#ifndef MP_PACKAGE_VALIDATION_OFFLINE_TEST
MP_AUTH_RESULT mp_package_validate_authenticated(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i){return run(o,i,&mp_auth_native_port,i?i->crypto:NULL);}
#else
MP_AUTH_RESULT mp_auth_offline_test_run(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i,const MP_AUTH_PORT *p,void *ctx){return run(o,i,p,ctx);}
#endif
MP_AUTH_RESULT mp_package_validation_finish(MP_AUTH_OPERATION *o){
 if(!o||o->flow.state!=2||!o->internal_port||!o->input)return MP_AUTH_INVALID;
 o->output_size=0;return (MP_AUTH_RESULT)sp_m1_finish(&o->flow,&binding,o);
}
size_t mp_package_validation_process_limit(void){return PAPACC_ED11_PROCESS_LIMIT;}
