#include "manual_package_validation_internal.h"
#include "../core/ed11_process_profile.h"
#include <stdint.h>
#include <string.h>
static int span(const void *p,size_t n){return p&&n&&n-1<=UINTPTR_MAX-(uintptr_t)p;}
static int overlaps(const void *p,size_t n,const void *q,size_t m){
 uintptr_t a=(uintptr_t)p,b=(uintptr_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);
}
static int stopped(const MP_AUTH_INPUT *i){return i->stopped&&i->stopped(i->stop_context);}
static void failure(MP_AUTH_OPERATION *o,int code){
 if(!o->first_failure_code){o->first_failure_stage=o->stage;o->first_failure_code=code;}
}
static void wipe(MP_AUTH_OPERATION *o){
 const MP_AUTH_PORT *p=o->internal_port;const MP_AUTH_INPUT *i=o->input;
 p->wipe(o->internal_context,o->key,sizeof(o->key));
 p->wipe(o->internal_context,i->quarantine,i->quarantine_capacity);
 p->wipe(o->internal_context,i->staging,i->staging_capacity);o->staging_size=0;
}
static MP_AUTH_RESULT abort_operation(MP_AUTH_OPERATION *o,MP_AUTH_RESULT result,int code){
 const MP_AUTH_PORT *p=o->internal_port;failure(o,code);o->output_size=0;
 if(code==MP_AUTH_STEP_UNCERTAIN||p->quiescent(o->internal_context)!=1){o->state=2;return MP_AUTH_RETAINED;}
 wipe(o);o->state=3;return result;
}
static MP_AUTH_RESULT stage(MP_AUTH_OPERATION *o,MP_AUTH_STAGE which,const MP_ENVELOPE *e,const MP_PAYLOAD *v){
 const MP_AUTH_PORT *p=o->internal_port;int code;o->stage=which;
 if(stopped(o->input))return abort_operation(o,MP_AUTH_STOPPED,MP_AUTH_STOPPED);
 if(which==MP_AUTH_KDF)o->kdf_invocations++;
 if(which==MP_AUTH_AEAD)o->aead_invocations++;
 if(which==MP_AUTH_CONTENT)o->content_invocations++;
 code=p->step(o->internal_context,which,o,e,v);
 if(code==MP_AUTH_STOPPED)return abort_operation(o,MP_AUTH_STOPPED,code);
 if(code!=MP_AUTH_STEP_OK)return abort_operation(o,MP_AUTH_DENIED,code);
 if(stopped(o->input))return abort_operation(o,MP_AUTH_STOPPED,MP_AUTH_STOPPED);
 return MP_AUTH_VALIDATED; /* internal stage-only marker, never public trust */
}
static int arguments(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i){
 const void *p[7];size_t n[7];unsigned a,b;
 if(!span(o,sizeof(*o))||!span(i,sizeof(*i)))return 0;
 p[0]=o;n[0]=sizeof(*o);p[1]=i;n[1]=sizeof(*i);
 p[2]=i->package.data;n[2]=i->package.size;p[3]=i->password.data;n[3]=i->password.size;
 p[4]=i->quarantine;n[4]=i->quarantine_capacity;p[5]=i->staging;n[5]=i->staging_capacity;p[6]=i->output;n[6]=i->output_capacity;
 if(n[2]>MP_INPUT_MAX||n[3]>128||n[4]>MP_INPUT_MAX||n[5]>MP_INPUT_MAX||n[6]>MP_INPUT_MAX)return 0;
 for(a=0;a<7;a++){if(!span(p[a],n[a]))return 0;for(b=0;b<a;b++)if(overlaps(p[a],n[a],p[b],n[b]))return 0;}
 return 1;
}
static int budget(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i){
 size_t total=sizeof(*o)+sizeof(*i); /* retained envelope/payload already inside operation */
 size_t n[7];unsigned j;
 n[0]=i->package.size;n[1]=i->password.size;n[2]=i->quarantine_capacity;
 n[3]=i->staging_capacity;n[4]=i->output_capacity;n[5]=i->other_reserved;n[6]=i->provider_reserved;
 for(j=0;j<7;j++){if(n[j]>(size_t)PAPACC_ED11_PROCESS_LIMIT||total>(size_t)PAPACC_ED11_PROCESS_LIMIT-n[j])return 0;total+=n[j];}
 o->managed_estimate=total;return 1;
}
static MP_AUTH_RESULT run(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i,const MP_AUTH_PORT *p,void *ctx){
 MP_ENVELOPE *e;MP_PAYLOAD *v;MP_RESULT parsed;MP_AUTH_RESULT result;
 if(!p||!p->step||!p->wipe||!p->quiescent||!arguments(o,i)||o->state)return MP_AUTH_INVALID;
 e=&o->envelope;v=&o->payload;
 o->state=1;o->input=i;o->internal_port=p;o->internal_context=ctx;o->output_size=0;
 o->first_failure_code=0;o->first_failure_stage=MP_AUTH_NONE;o->stage=MP_AUTH_STRUCTURE;
 o->kdf_invocations=0;o->aead_invocations=0;o->content_invocations=0;
 parsed=mp_envelope_inspect(i->package.data,i->package.size,e,stopped(i));
 if(parsed!=MP_DATA_ONLY){failure(o,parsed);o->state=3;return parsed==MP_STOPPED?MP_AUTH_STOPPED:MP_AUTH_INVALID;}
 o->stage=MP_AUTH_BUDGET;
 if(e->ciphertext.size>MP_ARCHIVE_MAX||i->quarantine_capacity<e->ciphertext.size||
 i->staging_capacity<e->ciphertext.size||i->output_capacity<e->ciphertext.size||!budget(o,i)){
 failure(o,MP_AUTH_LIMIT);o->state=3;return MP_AUTH_LIMIT;}
 /* Caller workspaces are acquired only after structural/budget checks. */
 wipe(o);
 result=stage(o,MP_AUTH_SIGNATURE,e,NULL);if(result!=MP_AUTH_VALIDATED)return result;
 result=stage(o,MP_AUTH_TRUST,e,NULL);if(result!=MP_AUTH_VALIDATED)return result;
 result=stage(o,MP_AUTH_KDF,e,NULL);if(result!=MP_AUTH_VALIDATED)return result;
 result=stage(o,MP_AUTH_AEAD,e,NULL);if(result!=MP_AUTH_VALIDATED)return result;
 o->stage=MP_AUTH_CONTENT;
 if(!o->staging_size||o->staging_size!=e->ciphertext.size)
 return abort_operation(o,MP_AUTH_DENIED,MP_AUTH_STEP_INVALID);
 parsed=mp_payload_inspect(i->staging,o->staging_size,v,stopped(i));
 if(parsed!=MP_DATA_ONLY)return abort_operation(o,parsed==MP_STOPPED?MP_AUTH_STOPPED:MP_AUTH_DENIED,parsed);
 result=stage(o,MP_AUTH_CONTENT,e,v);if(result!=MP_AUTH_VALIDATED)return result;
 result=stage(o,MP_AUTH_RECHECK_TRUST,e,v);if(result!=MP_AUTH_VALIDATED)return result;
 result=stage(o,MP_AUTH_RELEASE,e,v);if(result!=MP_AUTH_VALIDATED)return result;
 if(p->quiescent(ctx)!=1)return abort_operation(o,MP_AUTH_RETAINED,MP_AUTH_STEP_UNCERTAIN);
 if(stopped(i))return abort_operation(o,MP_AUTH_STOPPED,MP_AUTH_STOPPED);
 /* Sole publication point: caller output is untouched on all earlier paths. */
 memcpy(i->output,i->staging,o->staging_size);o->output_size=o->staging_size;
 wipe(o);o->state=3;
#ifdef MP_PACKAGE_VALIDATION_OFFLINE_TEST
 return MP_AUTH_SYNTHETIC_ONLY;
#else
 return MP_AUTH_VALIDATED;
#endif
}
#ifndef MP_PACKAGE_VALIDATION_OFFLINE_TEST
MP_AUTH_RESULT mp_package_validate_authenticated(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i){
 return run(o,i,&mp_auth_native_port,i?i->crypto:NULL);
}
#else
MP_AUTH_RESULT mp_auth_offline_test_run(MP_AUTH_OPERATION *o,const MP_AUTH_INPUT *i,const MP_AUTH_PORT *p,void *ctx){return run(o,i,p,ctx);}
#endif
MP_AUTH_RESULT mp_package_validation_finish(MP_AUTH_OPERATION *o){
 const MP_AUTH_PORT *p;if(!o||o->state!=2||!o->internal_port||!o->input)return MP_AUTH_INVALID;
 p=o->internal_port;if(p->quiescent(o->internal_context)!=1)return MP_AUTH_RETAINED;
 /* No repeat of uncertain step/release, no late plaintext publication. */
 wipe(o);o->output_size=0;o->state=3;return MP_AUTH_ABORTED;
}
size_t mp_package_validation_process_limit(void){return PAPACC_ED11_PROCESS_LIMIT;}
