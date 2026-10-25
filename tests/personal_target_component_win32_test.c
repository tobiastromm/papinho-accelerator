#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <bcrypt.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT o;
/* This function is reachable ONLY via explicitly reviewed LAB case ca-lab.
 * Real native key owner; modeled administrative/root authority stays absent. */
static int lab_ca(PAPACC_CA_KEY *ca,const void *holder)
{
 BYTE token_user[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE],service[SECURITY_MAX_SID_SIZE],private_blob[2048];
 DWORD bytes=0,service_bytes=sizeof(service),written=0;HANDLE token=NULL;SECURITY_STATUS status,export_status;
 TOKEN_USER *user=(TOKEN_USER *)token_user;
 static const BYTE authority[16]={0xcb,0x87,0x63,0x24,0x63,0xbb,0x48,0xdd,0x8a,0x34,0xfa,0x8e,0xa6,0xf3,0x91,0x1b};
 static const BYTE generation[16]={0x03,0x0f,0xa3,0x4d,0xd4,0x4f,0x41,0xe8,0xb1,0x43,0xb9,0xda,0x09,0x53,0x2c,0xf2};
 memset(ca,0,sizeof(*ca));memset(private_blob,0,sizeof(private_blob));
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return 0;
 if(!GetTokenInformation(token,TokenUser,token_user,sizeof(token_user),&bytes)){
  if(!CloseHandle(token)){ca->retained_token=token;ca->thread=GetCurrentThreadId();ca->state=PAPACC_CA_KEY_RETAINED;ca->error=NTE_FAIL;puts("LAB_TOKEN_CLOSE_UNKNOWN_RETAINED");}return 0;}
 if(!CloseHandle(token)){ca->retained_token=token;ca->thread=GetCurrentThreadId();ca->state=PAPACC_CA_KEY_RETAINED;ca->error=NTE_FAIL;puts("LAB_TOKEN_CLOSE_UNKNOWN_RETAINED");return 0;}
 if(!CreateWellKnownSid(WinLocalServiceSid,NULL,service,&service_bytes)||!IsValidSid(user->User.Sid))return 0;
 /* LocalService is the explicitly selected LAB exclusion identity only,
  * never guessed as the product's service identity. No export or certificate. */
 status=papacc_ca_key_create(ca,authority,generation,user->User.Sid,service);
 printf("LAB_CA status=%08lx created=%d finalization=%lu state=%u key_owned=%u provider_owned=%u\n",
  (unsigned long)status,ca->created,ca->diagnostic_finalization,(unsigned)ca->state,ca->key!=0,ca->provider!=0);
 printf("LAB_CA_FIRST function=%lu stage=%lu api=%lu original=%08lx\n",ca->first_failure.function,ca->first_failure.stage,ca->first_failure.native_api,(unsigned long)ca->first_failure.original_code);
 if(status!=ERROR_SUCCESS)return 0; /* no reopen, rename, replacement or retry */
 if(papacc_ca_key_validate(ca)!=ERROR_SUCCESS)return 0;
 export_status=NCryptExportKey(ca->key,0,BCRYPT_RSAPRIVATE_BLOB,NULL,private_blob,sizeof(private_blob),&written,0);
 SecureZeroMemory(private_blob,sizeof(private_blob));
 printf("LAB_PRIVATE_EXPORT_STATUS=%08lx\n",(unsigned long)export_status);
 if(export_status!=NTE_PERM){puts("STOP_LAB_PRIVATE_EXPORT_DENIAL_NOT_QUALIFIED");return 0;}
 if(papacc_ca_key_borrow(ca,holder)!=ERROR_SUCCESS||ca->borrower!=holder)return 0;
 if(papacc_ca_key_close(ca)==ERROR_SUCCESS||!ca->key||!ca->provider)return 0;
 if(papacc_ca_key_return(ca,holder)!=ERROR_SUCCESS)return 0;
 /* Exact disposable test key only. Successful NCryptDeleteKey consumes this
  * owned key handle. Failure retains it: no double-free or retry. */
 status=NCryptDeleteKey(ca->key,NCRYPT_SILENT_FLAG);
 if(status!=ERROR_SUCCESS){ca->state=PAPACC_CA_KEY_RETAINED;ca->error=status;puts("STOP_LAB_DELETE_UNKNOWN_KEY_PROVIDER_RETAINED");return 0;}
 ca->key=0;
 if(papacc_ca_key_close(ca)!=ERROR_SUCCESS)return 0;
 if(ca->state!=PAPACC_CA_KEY_CLOSED||ca->provider||!ca->close_confirmed)return 0;
 puts("PASS LAB_CA_NATIVE_CUSTODY_VALIDATED PRIVATE_EXPORT_DENIED BORROW_PROTECTED LAB_KEY_DELETION_CONFIRMED NO_CERT_NO_AUTHORITY");return 1;
}

#define CHECK(x) do{if(!(x)){printf("FAIL line=%u native=%08lx created=%u active=%d HANDLES_RETAINED_NOT_ASSUMED_RELEASED\n",(unsigned)__LINE__,o.pending_commit.native_error,o.pending_commit.bootstrap_created,o.pending_commit.active);return 1;}}while(0)
int main(int argc,char **argv)
{
 PAPACC_CONFIG_PENDING *p=&o.pending_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 const char *mode;char child[MAX_PATH];unsigned char id[32]={1},other[32]={2};
 static const unsigned char plan[]="format_version 1\nclient_admission DISABLED\n";
 BY_HANDLE_FILE_INFORMATION before,after;HANDLE held,probe=INVALID_HANDLE_VALUE;
 PAPACC_RESULT result;PAPACC_PERSONAL_CUSTODY_DATA q;PAPACC_CA_KEY ca;
 int preexisting,barrier,uncertain,closefail;
 memset(&before,0,sizeof(before));CHECK(argc==3);mode=argv[2];
 CHECK(strncmp(argv[1],"C:\\Projetos\\PapinhoAccelerator\\build\\personal-target-ca-qualification-20261022\\effects\\",87U)==0 ||
  strncmp(argv[1],"C:\\Projetos\\PapinhoAccelerator\\build\\personal-bootstrap-completion-20261022\\effects\\",
   sizeof("C:\\Projetos\\PapinhoAccelerator\\build\\personal-bootstrap-completion-20261022\\effects\\")-1U)==0);
 CHECK(!strstr(argv[1],".."));
 CHECK(sprintf_s(child,sizeof(child),"%s\\child",argv[1])>0);
 preexisting=!strcmp(mode,"exists-dir")||!strcmp(mode,"exists-file");
 barrier=!strcmp(mode,"stop-after");uncertain=!strcmp(mode,"unknown");closefail=!strcmp(mode,"closefail");
 /* The fixture parent is pre-created by the exact approved runner. Existing
  * child cases are also explicit fixture effects, before parent acquisition. */
 if(preexisting){
  if(!strcmp(mode,"exists-dir")){CHECK(CreateDirectoryA(child,NULL));probe=CreateFileA(child,FILE_READ_ATTRIBUTES,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);}
  else probe=CreateFileA(child,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
  CHECK(probe!=INVALID_HANDLE_VALUE&&GetFileInformationByHandle(probe,&before));CHECK(CloseHandle(probe));probe=INVALID_HANDLE_VALUE;
 }
 InitializeCriticalSection(&o.lock);o.initialized=1;o.writer=GetCurrentThreadId();
 p->file=INVALID_HANDLE_VALUE;p->checkpoint_only=93;n->phase=1U;
 /* Actual read-only/ADD_SUBDIRECTORY capability, not an administrator grant. */
 p->bootstrap_parent=CreateFileA(argv[1],FILE_READ_ATTRIBUTES|FILE_TRAVERSE|FILE_ADD_SUBDIRECTORY|SYNCHRONIZE,
  FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 CHECK(p->bootstrap_parent!=INVALID_HANDLE_VALUE&&p->bootstrap_parent);
 CHECK(GetFileInformationByHandle(p->bootstrap_parent,&n->parent));
 CHECK((n->parent.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&!(n->parent.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT));
 p->bootstrap_parent_volume=n->parent.dwVolumeSerialNumber;p->bootstrap_parent_high=n->parent.nFileIndexHigh;p->bootstrap_parent_low=n->parent.nFileIndexLow;
 held=p->bootstrap_parent;
 memcpy(n->plan_leaf,"child",sizeof("child"));memcpy(p->previous,"child",sizeof("child"));memcpy(p->next,plan,sizeof(plan)-1U);
 CHECK(papacc_personal_data_capture(&p->personal_data,id,plan,sizeof(plan)-1U));
 CHECK(papacc_personal_data_present(&p->personal_data)&&papacc_personal_data_confirm(&p->personal_data,id,plan,sizeof(plan)-1U));
 /* These are MODEL confirmations only. The product effect gate must deny. */
 CHECK(papacc_configuration_personal_target_begin(&o,id,sizeof(o)+PAPACC_PENDING_STACK_BYTES)==PAPACC_RESULT_NOT_SUPPORTED);
 q=papacc_configuration_personal_custody_data(&o,NULL);CHECK(q.ca==PAPACC_PERSONAL_CUSTODY_ABSENT&&q.effect_denied);
 if(!strcmp(mode,"model-ca")){
  memset(&ca,0,sizeof(ca));ca.state=PAPACC_CA_KEY_READY;
  q=papacc_configuration_personal_custody_data(&o,&ca);CHECK(q.ca==PAPACC_PERSONAL_CUSTODY_METADATA&&q.effect_denied&&!ca.key&&!ca.provider);
 }
 if(barrier){p->test_entered=CreateEventA(NULL,TRUE,FALSE,NULL);p->test_release=CreateEventA(NULL,TRUE,FALSE,NULL);CHECK(p->test_entered&&p->test_release);}
 if(uncertain)p->test_unknown=1;
 if(!strcmp(mode,"stop-before"))CHECK(papacc_configuration_stop(&o)==PAPACC_RESULT_OK);
 if(!strcmp(mode,"parent-mismatch"))p->bootstrap_parent_low^=1U; /* contradictory MODEL expected tuple; actual handle unchanged */
 result=papacc_configuration_personal_target_component_begin(&o,!strcmp(mode,"attempt-mismatch")?other:id,sizeof(o)+PAPACC_PENDING_STACK_BYTES);
 if(!strcmp(mode,"stop-before")||!strcmp(mode,"parent-mismatch")||!strcmp(mode,"attempt-mismatch")){
  CHECK(result!=PAPACC_RESULT_OK&&!p->active&&!p->thread&&!p->bootstrap_created&&!p->personal_data.consumed);
  CHECK(GetFileAttributesA(child)==INVALID_FILE_ATTRIBUTES&&GetLastError()==ERROR_FILE_NOT_FOUND);
 }else{
  CHECK(result==PAPACC_RESULT_OK&&p->personal_data.consumed&&p->bootstrap_parent==held);
  CHECK(papacc_configuration_personal_target_component_begin(&o,id,sizeof(o)+PAPACC_PENDING_STACK_BYTES)!=PAPACC_RESULT_OK);
  if(barrier){
   CHECK(WaitForSingleObject(p->test_entered,10000U)==WAIT_OBJECT_0&&p->bootstrap_created);
   CHECK(papacc_configuration_stop(&o)==PAPACC_RESULT_OK);
   CHECK(papacc_configuration_personal_native_close(&o)!=PAPACC_RESULT_OK);
   CHECK(papacc_configuration_personal_data_retire(&o)!=PAPACC_RESULT_OK);
   CHECK(SetEvent(p->test_release));
  }
  /* A diagnostic timeout is FAIL/retention, never cancellation or release. */
  CHECK(WaitForSingleObject(p->thread,10000U)==WAIT_OBJECT_0);
  result=papacc_configuration_personal_handoff_poll(&o);
  if(uncertain){
   CHECK(result!=PAPACC_RESULT_OK&&o.unresolved&&p->active&&p->thread&&p->bootstrap_container);
   CHECK(GetFileInformationByHandle(p->bootstrap_container,&after));
   CHECK(papacc_configuration_personal_native_close(&o)!=PAPACC_RESULT_OK);
   CHECK(papacc_configuration_personal_data_retire(&o)!=PAPACC_RESULT_OK);
   CHECK(papacc_configuration_personal_target_component_begin(&o,id,sizeof(o)+PAPACC_PENDING_STACK_BYTES)!=PAPACC_RESULT_OK);
   puts("PASS LAB_NATIVE_UNKNOWN_INJECTED_RETAINED WHOLE_OWNER_CHARGED NO_DESTROYED_NO_ROLLBACK");return 0;
  }
  CHECK(result==PAPACC_RESULT_OK&&!p->active&&!p->thread&&p->bootstrap_parent==held);
  if(preexisting){
   CHECK(!p->bootstrap_created&&!p->bootstrap_container&&p->result==PAPACC_CAS_CONFLICT);
   probe=CreateFileA(child,FILE_READ_ATTRIBUTES,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
   CHECK(probe!=INVALID_HANDLE_VALUE&&GetFileInformationByHandle(probe,&after));CHECK(CloseHandle(probe));
   CHECK(before.dwVolumeSerialNumber==after.dwVolumeSerialNumber&&before.nFileIndexHigh==after.nFileIndexHigh&&before.nFileIndexLow==after.nFileIndexLow);
  }else{
   CHECK(p->bootstrap_created&&p->bootstrap_container&&GetFileInformationByHandle(p->bootstrap_container,&after));
   CHECK(after.dwVolumeSerialNumber==p->bootstrap_volume&&after.nFileIndexHigh==p->bootstrap_high&&after.nFileIndexLow==p->bootstrap_low);
   probe=CreateFileA(child,DELETE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
   CHECK(probe==INVALID_HANDLE_VALUE); /* no DELETE lease while custody lives */
  }
  CHECK(!p->bootstrap_admin&&!o.published&&!o.committed_bytes&&!o.committed_pending);
 }
 if(!strcmp(mode,"ca-lab")){CHECK(p->bootstrap_created&&p->bootstrap_container&&!p->active&&!p->thread);CHECK(lab_ca(&ca,&o));}
 if(closefail)p->test_unknown=2;
 result=papacc_configuration_personal_native_close(&o);
 if(closefail){
  CHECK(result==PAPACC_RESULT_INTERNAL_ERROR&&o.unresolved&&p->bootstrap_container&&p->bootstrap_parent);
  CHECK(GetFileInformationByHandle(p->bootstrap_container,&after));
  CHECK(papacc_configuration_personal_native_close(&o)!=PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_data_retire(&o)!=PAPACC_RESULT_OK);
  puts("PASS LAB_NATIVE_CLOSE_FAILURE_INJECTED_RETAINED NO_RETRY_NO_DOUBLE_FREE");return 0;
 }
 CHECK(result==PAPACC_RESULT_OK&&!p->bootstrap_container&&!p->bootstrap_parent);
 if(p->test_entered)CHECK(CloseHandle(p->test_entered));if(p->test_release)CHECK(CloseHandle(p->test_release));
 CHECK(papacc_configuration_personal_data_retire(&o)==PAPACC_RESULT_OK);
 CHECK(!o.published&&!o.committed_bytes&&!o.committed_pending);
 DeleteCriticalSection(&o.lock);
 printf("PASS LAB_NATIVE_TARGET case=%s created=%u MODEL_AUTH_NOT_NATIVE CA_ABSENT NO_CURRENT_NO_COMMIT\n",mode,p->bootstrap_created);return 0;
}
