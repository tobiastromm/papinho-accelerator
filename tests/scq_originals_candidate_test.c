/* Actual production acquire-originals wrapper; native effects are mocked. */
#include "server_configuration_commit_win32.h"
#include "personal_scq_handle_policy_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT owner;
static PAPACC_SCQ_PLAN_DATA model_plan;
static unsigned calls,fault,registered,decode_error,checks;
static const PAPACC_SCQ_OWNER_DECISION *reg(PAPACC_CONFIGURATION_COMMIT *o){(void)o;return registered?(const PAPACC_SCQ_OWNER_DECISION *)&model_plan:NULL;}
static DWORD decode(const BYTE *p,DWORD n,PAPACC_SCQ_PLAN_DATA *v){(void)p;(void)n;*v=model_plan;return decode_error?ERROR_INVALID_DATA:0;}
static PAPACC_RESULT capture(PAPACC_CONFIGURATION_COMMIT *o,const HANDLE h[3]){(void)o;(void)h;calls++;return fault==1U?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static PAPACC_RESULT contents(PAPACC_CONFIGURATION_COMMIT *o){(void)o;calls++;return fault==2U?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static DWORD selection(PAPACC_CONFIGURATION_COMMIT *o){(void)o;calls++;return fault==3U?ERROR_ACCESS_DENIED:0;}
static PAPACC_RESULT admin(PAPACC_CONFIGURATION_COMMIT *o){(void)o;calls++;return fault==4U?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
#define personal_scq_registered_owner_decision reg
#define scqp_decode_fields decode
#define papacc_configuration_personal_scq_capture_inputs capture
#define papacc_configuration_personal_scq_acquire_contents contents
#define personal_scq_selection_capture selection
#define papacc_configuration_personal_scq_admin_prepare admin
#define papacc_configuration_personal_scq_session_acquire_originals actual_acquire
#include "../apps/server/personal_scq_acquire_originals_win32.inc"
#define C(x) do{checks++;if(!(x)){fprintf(stderr,"FAIL line=%u checks=%u\n",(unsigned)__LINE__,checks);return 1;}}while(0)
static void reset(void){PAPACC_PERSONAL_NATIVE_CONTEXT *n;memset(&owner,0,sizeof(owner));memset(&model_plan,0,sizeof(model_plan));calls=fault=registered=decode_error=0;owner.writer=GetCurrentThreadId();n=&owner.pending_commit.personal_native;n->scq_profile_data.pre_readiness=1;n->scq_profile_data.waiting_runtime=1;n->scq_profile_data.read_diagnostic=1;n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;n->install_paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;n->install_paths.thread=GetCurrentThreadId();n->install_paths.selected_role=1;n->install_paths.original_chain=(void *)&model_plan;n->install_paths.read_diagnostic=1;model_plan.purpose=PAPACC_SCQ_PURPOSE_READ;}
int main(void){unsigned i;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&owner.pending_commit.personal_native;HANDLE refs[3]={0};
 reset();C(actual_acquire(&owner,refs)==PAPACC_RESULT_OK&&calls==4);
 reset();model_plan.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;n->scq_profile_data.read_diagnostic=0;n->install_paths.read_diagnostic=0;C(actual_acquire(&owner,refs)==PAPACC_RESULT_OK&&calls==4);
 for(i=0;i<13U;i++){reset();switch(i){case 0:n->scq_profile_data.read_diagnostic=0;break;case 1:n->install_paths.read_diagnostic=0;break;case 2:n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;break;case 3:n->install_paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;break;case 4:n->install_paths.original_chain=NULL;break;case 5:n->install_paths.thread=owner.writer+1;break;case 6:n->install_paths.selected_role=2;break;case 7:n->install_paths.phase=0;break;case 8:n->scq_profile_data.pre_readiness=0;break;case 9:n->scq_profile_data.waiting_runtime=0;break;case 10:decode_error=1;break;case 11:model_plan.purpose=99;break;case 12:owner.pending_commit.stop=1;break;}C(actual_acquire(&owner,refs)!=PAPACC_RESULT_OK&&calls==0);}
 for(i=1;i<=4U;i++){reset();fault=i;C(actual_acquire(&owner,refs)!=PAPACC_RESULT_OK&&calls==i);}
 reset();registered=1;C(actual_acquire(&owner,refs)==PAPACC_RESULT_OK&&calls==3); /* existing-source route unchanged */
 printf("PASS ORIGINALS_CANDIDATE checks=%u READ_EXPLICIT_TO_PRESERVED_ORDINARY_READ_NEW_INSTALL_DENY ORIGINAL_SELECTION_STILL_REQUIRED NO_NATIVE_EFFECTS\n",checks);return 0;}
