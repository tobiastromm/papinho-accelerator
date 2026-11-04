#include <windows.h>
#include "c2_operation_win32.h"
#include "server_credential_revocation_win32.h"
#include <stdio.h>
#include <string.h>
#include "../apps/server/server_credential_revocation_win32.inc"
/* Isolated memory cohort, no authentication, token grant, socket or root. */
static PAPACC_C2_OPERATION_MANAGER manager;
static PAPACC_C2_SESSION sessions[4],unrelated;
static PAPACC_C2_OPERATION operation_before;
static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static PAPACC_RESULT reject_policy(void *p,const PAPACC_C2_SESSION *s,const PAPACC_U8 *bytes,PAPACC_SIZE n,PAPACC_BOOL *permit)
{(void)p;(void)s;(void)bytes;(void)n;*permit=PAPACC_FALSE;return PAPACC_RESULT_OK;}
static PAPACC_RESULT reject_capacity(void *p,PAPACC_SIZE n,PAPACC_BOOL *permit)
{(void)p;(void)n;*permit=PAPACC_FALSE;return PAPACC_RESULT_OK;}
static int setup(void){
 unsigned i;memset(&manager,0,sizeof(manager));memset(sessions,0,sizeof(sessions));
 CHECK(papacc_c2_operation_manager_init(&manager,GetCurrentThreadId(),reject_policy,reject_capacity,NULL)==0);
 for(i=0;i<2U;i++){
  CHECK(papacc_c2_session_init(&sessions[i],manager.owner,i+1U,42U,77U,1U,PAPACC_TRUE,PAPACC_TRUE,PAPACC_FALSE)==0);
  manager.sessions[i].session=&sessions[i];
  manager.operations[i].state=PAPACC_C2_OP_APPLICATION;manager.operations[i].session=&sessions[i];
  manager.operations[i].tokens.generated=PAPACC_TRUE;manager.operations[i].tokens.owner=manager.owner;
  manager.operations[i].tokens.borrowers=1U;memset(manager.operations[i].tokens.bytes,0xa5,sizeof(manager.operations[i].tokens.bytes));
 }
 return 0;
}
int main(void){
 PAPACC_CREDENTIAL_REVOCATION_RECEIPT receipt;PAPACC_U64 deadline;PAPACC_U32 owner;
 CHECK(setup()==0);memset(&receipt,0,sizeof(receipt));unrelated=sessions[1];operation_before=manager.operations[1];
 manager.operations[0].first_cause=7U;
 CHECK(credential_stop_cohort(&manager,sessions,1U,&receipt)==0);
 CHECK(sessions[0].state==PAPACC_C2_SESSION_REVOKED&&manager.operations[0].revoked&&manager.operations[0].tokens.revoked);
 CHECK(manager.operations[0].state==PAPACC_C2_OP_TERMINATING&&manager.operations[0].tokens.borrowers==1U&&manager.operations[0].tokens.bytes[0][0]==0xa5);
 CHECK(manager.operations[0].first_cause==7U&&!memcmp(&sessions[1],&unrelated,sizeof(unrelated))&&!memcmp(&manager.operations[1],&operation_before,sizeof(operation_before)));
 CHECK(receipt.sessions_invalidated==1U&&receipt.operations_stop_requested==1U&&!receipt.quiescence_proven&&!receipt.close_proven&&!receipt.controls_stop_requested);
 deadline=manager.operations[0].cleanup_deadline_ms;memset(&receipt,0,sizeof(receipt));
 CHECK(credential_stop_cohort(&manager,sessions,1U,&receipt)==0&&manager.operations[0].cleanup_deadline_ms==deadline);
 manager.operations[0].state=PAPACC_C2_OP_RETAINED;CHECK(credential_stop_cohort(&manager,sessions,1U,&receipt)==0&&manager.operations[0].state==PAPACC_C2_OP_RETAINED);
 CHECK(setup()==0);memset(&receipt,0,sizeof(receipt));manager.operations[0].tokens.generated=PAPACC_FALSE;
 CHECK(credential_stop_cohort(&manager,sessions,1U,&receipt)!=0&&receipt.uncertain&&receipt.first_error&&!receipt.close_proven&&sessions[0].state==PAPACC_C2_SESSION_REVOKED);
 CHECK(setup()==0);memset(&receipt,0,sizeof(receipt));owner=manager.owner;manager.owner++;
 CHECK(credential_stop_cohort(&manager,sessions,1U,&receipt)!=0&&sessions[0].state==PAPACC_C2_SESSION_CONFIGURED);manager.owner=owner;
 CHECK(credential_stop_cohort(&manager,sessions,16U,&receipt)!=0);
 manager.sessions[0].session=NULL;CHECK(credential_stop_cohort(&manager,sessions,1U,&receipt)!=0&&sessions[0].state==PAPACC_C2_SESSION_CONFIGURED);
 puts("PASS effect_only_cohort; SAME_PRINCIPAL_OTHER_CREDENTIAL_PRESERVED; RETAINED_BORROWS; NO_AUTHORITY_NO_NETWORK_NO_CLOSE");
 printf("checks=%u\n",checks);return 0;
}
