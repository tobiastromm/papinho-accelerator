/* Actual offer wrapper. Operational APIs/source/path checks modeled, no KSP/SCM/pipe. */
#include "server_configuration_commit_win32.h"
#include "personal_scq_handle_policy_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT own;
static PAPACC_SCQ_PLAN_DATA model_plan;
static unsigned fault,calls,legacy,read_calls,to_calls,offered,checks;
static int current(PAPACC_CONFIGURATION_COMMIT *o){return o&&!o->stopped&&!o->pending_commit.stop&&fault!=1;}
static DWORD decode(const BYTE *b,DWORD n,PAPACC_SCQ_PLAN_DATA *p){(void)b;(void)n;*p=model_plan;return fault==2?ERROR_INVALID_DATA:0;}
static PAPACC_RESULT observe(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_SCQ_RUNTIME_OBSERVATION *r){(void)o;memset(r,0,sizeof(*r));r->known=fault==4?0:PAPACC_SCQ_OBS_COMPLETE;return fault==3?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static DWORD paths(const PAPACC_PERSONAL_INSTALL_PATHS *p,DWORD purpose,volatile LONG *stop){calls++;return fault==5||*stop||!PAPACC_SCQ_EXISTING_PATH_PURPOSE(p,purpose)?ERROR_ACCESS_DENIED:0;}
static DWORD prep(PAPACC_PERSONAL_SCQ_DISPATCH *d,const PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const BYTE a[32],const WCHAR *k){(void)d;(void)r;(void)a;(void)k;read_calls++;return fault==6?ERROR_ACCESS_DENIED:0;}
static DWORD to(PAPACC_PERSONAL_SCQ_DISPATCH *d,const PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const BYTE a[32],const WCHAR *k){(void)d;(void)r;(void)a;(void)k;to_calls++;return 0;}
static PAPACC_RESULT generic(PAPACC_CONFIGURATION_COMMIT *o,const BYTE a[32],const WCHAR *k){(void)o;(void)a;(void)k;legacy++;return PAPACC_RESULT_OK;}
static DWORD offer(PAPACC_SCQ_SESSION_DRIVER *s,const void *o,const PAPACC_SCQ_PEER_DATA *a,const PAPACC_SCQ_PEER_DATA *r,const PAPACC_SCQ_REF_DATA *refs,volatile LONG *stop){(void)s;(void)o;(void)a;(void)r;(void)refs;(void)stop;offered++;return fault==7?ERROR_ACCESS_DENIED:0;}
#define personal_scq_session_native_current current
#define scqp_decode_fields decode
#define papacc_configuration_personal_scq_observe_image_data observe
#define personal_scq_paths_current paths
#define papacc_personal_scq_prepare prep
#define papacc_personal_scq_prepare_transport to
#define papacc_configuration_personal_scq_prepare_data generic
#define scqs_offer offer
#define papacc_configuration_personal_scq_session_offer_data actual_offer
#include "../apps/server/personal_scq_offer_data_win32.inc"
#define C(x) do{checks++;if(!(x)){fprintf(stderr,"FAIL line%u checks%u\n",(unsigned)__LINE__,checks);return 1;}}while(0)
static void reset(void){PAPACC_PERSONAL_NATIVE_CONTEXT *n;memset(&own,0,sizeof(own));memset(&model_plan,0,sizeof(model_plan));fault=calls=legacy=read_calls=to_calls=offered=0;n=&own.pending_commit.personal_native;model_plan.purpose=PAPACC_SCQ_PURPOSE_READ;n->scq_profile_data.read_diagnostic=1;n->scq_profile_data.pre_readiness=1;n->install_paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;n->install_paths.thread=GetCurrentThreadId();n->install_paths.selected_role=1;n->install_paths.original_chain=(void *)&model_plan;n->install_paths.read_diagnostic=1;n->scq_read_diagnostic.phase=2;n->scq_session.read_act=&n->scq_read_diagnostic;}
int main(void){unsigned i;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&own.pending_commit.personal_native;
 reset();C(actual_offer(&own)==PAPACC_RESULT_OK&&read_calls==1&&offered==1&&legacy==0&&to_calls==0);
 for(i=0;i<10;i++){reset();switch(i){case 0:n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;break;case 1:n->install_paths.read_diagnostic=0;break;case 2:n->install_paths.original_chain=NULL;break;case 3:n->install_paths.thread=GetCurrentThreadId()+1;break;case 4:n->install_paths.selected_role=2;break;case 5:n->scq_profile_data.pre_readiness=0;break;case 6:n->scq_session.read_act=NULL;break;case 7:n->scq_read_diagnostic.phase=1;break;case 8:model_plan.purpose=99;break;case 9:own.pending_commit.stop=1;break;}C(actual_offer(&own)!=PAPACC_RESULT_OK&&read_calls==0&&offered==0&&legacy==0);}
 for(i=1;i<=7;i++){reset();fault=i;C(actual_offer(&own)!=PAPACC_RESULT_OK);C(offered==(unsigned)(i==7U));}
 reset();model_plan.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;n->install_paths.read_diagnostic=0;n->scq_profile_data.read_diagnostic=0;C(actual_offer(&own)==PAPACC_RESULT_OK&&to_calls==1&&read_calls==0&&offered==1);
 reset();n->scq_profile_data.read_diagnostic=0;n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;C(actual_offer(&own)==PAPACC_RESULT_OK&&legacy==1&&read_calls==0); /* existing generic route unchanged */
 printf("PASS OFFER_WRAPPER checks=%u READ_P_CONFIRMATION_SELECTED_ORIGINAL_GUARDS_TO_AND_GENERIC_UNCHANGED NO_KSP_NO_NATIVE_EFFECTS\n",checks);return 0;}
