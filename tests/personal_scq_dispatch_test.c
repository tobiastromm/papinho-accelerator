#define PAPACC_CNG_PROBE_NO_MAIN 1
#include "personal_cng_probe_test.c"
#include "../apps/server/personal_scq_dispatch_win32.inc"
/* Original-reference APIs modeled; origin acquisition is NOT granted. */
static DWORD receiver_admin_copy_model,receiver_admin_closed_mask;
static DWORD receiver_identity_alias;static DWORD receiver_fault;static ULONGLONG receiver_now=100;
static DWORD WINAPI receiver_type(HANDLE h){(void)h;return receiver_fault==1?FILE_TYPE_PIPE:FILE_TYPE_DISK;}
static BOOL WINAPI receiver_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION out)
{memset(out,0,sizeof(*out));out->dwVolumeSerialNumber=10;out->nFileIndexLow=(DWORD)(ULONG_PTR)h;if(receiver_identity_alias&&(ULONG_PTR)h>=500U)out->nFileIndexLow%=100U;out->nFileSizeLow=8;out->nNumberOfLinks=1;
 if(receiver_fault==2)out->nFileIndexLow++;if(receiver_fault==3)out->dwFileAttributes=FILE_ATTRIBUTE_REPARSE_POINT;
 return receiver_fault!=4;}
static DWORD WINAPI receiver_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return receiver_fault==5?WAIT_OBJECT_0:WAIT_TIMEOUT;}
static ULONGLONG WINAPI receiver_clock(void){return receiver_now;}
static DWORD WINAPI receiver_pid(HANDLE h){return h==(HANDLE)44||((h==(HANDLE)544||h==(HANDLE)644)&&receiver_admin_copy_model&&!(receiver_admin_closed_mask&(h==(HANDLE)544?1U:4U)))?88:model_pid(h);}
static BOOL WINAPI receiver_token(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID out,DWORD cap,PDWORD n)
{
 PSID saved=identity_expected;BOOL result;DWORD copied=h==(HANDLE)645;DWORD admin_n=SECURITY_MAX_SID_SIZE,interactive_n=SECURITY_MAX_SID_SIZE;TOKEN_GROUPS *g=out;BYTE *bytes=out;
 if(receiver_admin_copy_model&&(h==(HANDLE)545||copied)){
  if(receiver_admin_closed_mask&(copied?8U:2U))return FALSE;h=(HANDLE)45;
 }
 if(h==(HANDLE)45){
  if(kind==TokenStatistics){result=model_info(h,kind,out,cap,n);((TOKEN_STATISTICS *)out)->GroupCount=receiver_fault==11?3:2;return result;}
  if(kind==TokenElevation){((TOKEN_ELEVATION *)out)->TokenIsElevated=1;*n=sizeof(TOKEN_ELEVATION);return TRUE;}
  if(kind==TokenElevationType){*(TOKEN_ELEVATION_TYPE *)out=TokenElevationTypeFull;*n=sizeof(TOKEN_ELEVATION_TYPE);return TRUE;}
  if(kind==TokenSessionId){*(DWORD *)out=receiver_fault==12?2:1;*n=sizeof(DWORD);return TRUE;}
  if(kind==TokenGroups){
   if(cap<sizeof(TOKEN_GROUPS)+sizeof(SID_AND_ATTRIBUTES)+2*SECURITY_MAX_SID_SIZE)return FALSE;
   g->GroupCount=2;g->Groups[0].Sid=bytes+sizeof(TOKEN_GROUPS)+sizeof(SID_AND_ATTRIBUTES);g->Groups[1].Sid=(BYTE *)g->Groups[0].Sid+SECURITY_MAX_SID_SIZE;
   if(!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,g->Groups[0].Sid,&admin_n)||!CreateWellKnownSid(WinInteractiveSid,NULL,g->Groups[1].Sid,&interactive_n))return FALSE;
   g->Groups[0].Attributes=g->Groups[1].Attributes=SE_GROUP_ENABLED;if(receiver_fault==7)g->Groups[0].Attributes=0;if(receiver_fault==8)g->Groups[0].Attributes|=SE_GROUP_USE_FOR_DENY_ONLY;if(receiver_fault==9){*n=FIELD_OFFSET(TOKEN_GROUPS,Groups);return TRUE;}*n=sizeof(TOKEN_GROUPS)+sizeof(SID_AND_ATTRIBUTES)+2*SECURITY_MAX_SID_SIZE;return TRUE;}
  if(kind==TokenRestrictedSids&&receiver_fault==10){g->GroupCount=1;*n=FIELD_OFFSET(TOKEN_GROUPS,Groups);return TRUE;}
  if(kind==TokenUser)identity_expected=receiver_fault==6?virtual_sid:sam_sid;
 }
 result=model_info(h,kind,out,cap,n);identity_expected=saved;return result;
}
#undef GetProcessId
#undef GetTokenInformation
#define GetProcessId receiver_pid
#define GetTokenInformation receiver_token
#define GetFileType receiver_type
#define GetFileInformationByHandle receiver_file
#undef WaitForSingleObject
#define WaitForSingleObject receiver_wait
#undef GetTickCount64
#define GetTickCount64 receiver_clock
#include "../apps/server/personal_scq_handoff_win32.inc"
#undef GetFileType
#undef GetFileInformationByHandle
#undef WaitForSingleObject
#undef GetTickCount64
typedef struct RECEIVER_THREAD_WORK {PAPACC_PERSONAL_SCQ_RECEIVER *receiver;const void *owner;volatile LONG *stop;} RECEIVER_THREAD_WORK;
static DWORD WINAPI receiver_foreign(LPVOID p)
{RECEIVER_THREAD_WORK *w=p;return personal_scq_receive_consume(w->receiver,w->owner,w->stop);}
static int receiver_cases(void)
{
 PAPACC_PERSONAL_RUNTIME_IDENTITY r;PAPACC_PERSONAL_SCQ_DISPATCH d;
 PAPACC_PERSONAL_SCQ_RECEIVER v,second;PAPACC_PERSONAL_SCQ_ORIGINAL source,copy;unsigned i;DWORD e;int owner;
 for(i=0;i<24;i++){
  reset(&r,0,2);qfault=0;qstop=0;receiver_fault=0;receiver_now=100;
  C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);
  memset(&d,0,sizeof(d));memset(&v,0,sizeof(v));memset(&source,0,sizeof(source));memset(&second,0,sizeof(second));
  {BYTE a[32]={1};C(!papacc_personal_scq_prepare(&d,&r,a,fixture_name));}
  source.original=&source;source.controller=&owner;source.recipient=&owner;source.purpose=1;source.deadline=200;source.controller_consumed=1;source.operator_pid=88;source.operator_creation.dwLowDateTime=99;
  source.request=(HANDLE)41;source.decision=(HANDLE)42;source.image=(HANDLE)43;source.operator_process=(HANDLE)44;source.operator_token=(HANDLE)45;
  C(receiver_file(source.request,&source.request_id));C(receiver_file(source.decision,&source.decision_id));C(receiver_file(source.image,&source.image_id));
  source.operator_sid_bytes=GetLengthSid(sam_sid);C(CopySid(sizeof(source.operator_sid),source.operator_sid,sam_sid));
  {DWORD z;C(model_info(source.operator_token,TokenStatistics,&source.operator_statistics,sizeof(source.operator_statistics),&z));}
  source.runtime_pid=r.pid;source.runtime_creation=r.creation;source.runtime_token=r.statistics;
  memcpy(source.association,d.association,32);wcscpy_s(source.key,112,fixture_name);
  if(i>=1&&i<=5)receiver_fault=i;
  if(i==6)source.recipient=&r;if(i==7)source.purpose=2;if(i==8)source.deadline=100;
  if(i==9)source.consumed=1;if(i==10)source.borrowed=1;if(i==11)source.runtime_pid++;
  if(i==12)source.runtime_creation.dwLowDateTime++;if(i==13)source.runtime_token.ModifiedId.LowPart++;
  if(i==14)source.association[0]++;if(i==15)source.key[30]=L'3';if(i==16)qstop=1;
  copy=source;
  e=personal_scq_receive_original(&v,i==17?NULL:i==18?&copy:&source,&owner,&d,&r,&qstop);
  if(i>=1&&i<=18){C(e!=0&&!v.original);C(personal_scq_receive_original(&v,&source,&owner,&d,&r,&qstop)==ERROR_INVALID_STATE);}
  else{
   C(!e&&v.original==&source&&source.borrowed&&!source.consumed);
   {PAPACC_PERSONAL_SCQ_RECEIVER copied=v;C(personal_scq_receive_consume(&copied,&owner,&qstop)==ERROR_INVALID_STATE);C(personal_scq_receive_finish(&copied,&owner,1)==ERROR_INVALID_STATE&&!source.consumed&&source.borrowed);}
   C(personal_scq_receive_original(&second,&source,&owner,&d,&r,&qstop)==PAPACC_SCQ_RECEIVE_CONSUMED);
   if(i==0){RECEIVER_THREAD_WORK work;HANDLE worker;DWORD result;
    work.receiver=&v;work.owner=&owner;work.stop=&qstop;use_native_thread=1;
    worker=CreateThread(NULL,0,receiver_foreign,&work,0,NULL);C(worker!=NULL);
    C(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE);
    C(native_thread_close(worker));use_native_thread=0;C(!source.consumed&&source.borrowed&&v.phase==1);
   }
   if(i==19)qstop=1;if(i==20)receiver_now=200;if(i==21)source.purpose=2;if(i==22)receiver_fault=2;
   e=personal_scq_receive_consume(&v,&owner,&qstop);
   C(source.consumed==1);if(i>=19&&i<=22)C(e!=0);else C(!e);
   C(personal_scq_receive_consume(&v,&owner,&qstop)==ERROR_INVALID_STATE);
   C(personal_scq_receive_finish(&v,&owner,i!=23)==(DWORD)(i==23?PAPACC_SCQ_RECEIVE_CLOSE_UNKNOWN:0));
   if(i==23){C(v.phase==4&&source.retained&&source.borrowed&&v.original==&source);C(personal_scq_receive_finish(&v,&owner,1)==ERROR_INVALID_STATE);}
   else C(v.phase==3&&!v.original&&!source.borrowed&&source.consumed);
  }
  receiver_fault=0;fault=0;C(papacc_personal_runtime_close(&r)==0);
 }
 printf("PASS SCQ_RECEIVER cases=24 ORIGINAL_REFERENCE_MODEL NOT_NATIVE_ORIGIN_PROOF bytes=%zu\n",sizeof(v));return 0;
}
static PAPACC_PERSONAL_SCQ_DISPATCH threaded;
static PAPACC_PERSONAL_RUNTIME_IDENTITY threaded_runtime;
static PAPACC_PERSONAL_CNG_PROBE threaded_probe;
static BYTE thread_association[32]={1};
static DWORD WINAPI foreign_dispatch(LPVOID p)
{(void)p;return personal_scq_dispatch_body(&threaded,&threaded_runtime,&threaded_probe,thread_association,&qstop);}
#ifndef PAPACC_SCQ_DISPATCH_NO_MAIN
int main(void)
{
 PAPACC_PERSONAL_RUNTIME_IDENTITY r;PAPACC_PERSONAL_CNG_PROBE probe;PAPACC_PERSONAL_SCQ_DISPATCH d;
 BYTE association[32]={1},wrong[32]={2};WCHAR name[112];unsigned i,before;HANDLE other;DWORD code;
 checks=0;C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(receiver_cases()==0);
 for(i=0;i<8U;i++){
  WCHAR empty[112]={0};reset(&r,0,2);runtime_service_state=SERVICE_START_PENDING;
  C(!papacc_personal_runtime_capture_closed(&r,(HANDLE)1,virtual_sid,1U));
  memset(&probe,0,sizeof(probe));memset(&d,0,sizeof(d));qfault=qopens=qproviders=qfrees=0;qstop=0;
  C(!papacc_personal_scq_prepare_transport(&d,&r,association,empty)&&d.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY);
  if(i==1U)wcscpy_s(d.name,112,fixture_name);if(i==2U)r.closed_preparation=0;if(i==3U)qstop=1;
  if(i==4U)d.purpose=3U;if(i==5U)d.pid++;if(i==6U)d.association[0]++;
  if(i==7U){wcscpy_s(d.name,112,fixture_name);r.closed_preparation=0U;runtime_service_state=SERVICE_RUNNING;}
  C(personal_scq_dispatch_body(&d,&r,&probe,association,&qstop)==PAPACC_CNG_PROBE_LOCAL_DENY);
  C(!qproviders&&!qopens&&!probe.phase&&!probe.provider&&!probe.key);
  {unsigned k;for(k=0;k<112U;k++){memset(&d,0,sizeof(d));empty[k]=L'X';C(papacc_personal_scq_prepare_transport(&d,&r,association,empty)!=0);empty[k]=0;}}
  C(!papacc_personal_runtime_close(&r));runtime_service_state=SERVICE_RUNNING;
 }

 for(i=0;i<7;i++){
  reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);
  memset(&probe,0,sizeof(probe));memset(&d,0,sizeof(d));qfault=qopens=qproviders=qfrees=0;qstop=0;wcscpy_s(name,112,fixture_name);
  C(papacc_personal_scq_prepare(&d,&r,association,name)==0&&d.phase==1);
  name[wcslen(name)-1]=L'3';C(!wcscmp(d.name,fixture_name));C(papacc_personal_scq_prepare(&d,&r,association,fixture_name)!=0);
  if(i==1)qstop=1;if(i==2)r.pid++;if(i==3)r.creation.dwLowDateTime++;if(i==4)r.statistics.ModifiedId.LowPart++;if(i==5)qfault=9;
  code=personal_scq_dispatch_body(&d,&r,&probe,i==6?wrong:association,&qstop);
  if(i==0)C(code==PAPACC_CNG_PROBE_PERMISSION_OBSERVED&&d.phase==3&&qopens==1);
  else if(i==1)C(code==PAPACC_CNG_PROBE_STOPPED&&d.phase==3&&qproviders==0);
  else if(i==5)C(d.phase==4&&d.result==PAPACC_CNG_PROBE_CLOSE_UNKNOWN&&probe.provider);
  else C(code==PAPACC_CNG_PROBE_QUERY_FAILURE&&d.phase==3&&qproviders==0);
  before=qopens;C(personal_scq_dispatch_body(&d,&r,&probe,association,&qstop)==ERROR_INVALID_STATE&&qopens==before);
  if(i!=5){fault=0;C(papacc_personal_runtime_close(&r)==0);}
 }
 reset(&threaded_runtime,0,2);use_native_thread=1;qfault=0;qstop=0;qopens=qproviders=0;
 C(papacc_personal_runtime_capture(&threaded_runtime,(HANDLE)1,virtual_sid,2,1)==0);
 C(papacc_personal_scq_prepare(&threaded,&threaded_runtime,thread_association,fixture_name)==0);
 other=CreateThread(NULL,0,foreign_dispatch,NULL,0,NULL);C(other!=NULL);C(WaitForSingleObject(other,10000)==WAIT_OBJECT_0);
 C(GetExitCodeThread(other,&code)&&code==ERROR_INVALID_STATE&&threaded.phase==1&&qopens==0&&qproviders==0);C(native_thread_close(other));
 C(papacc_personal_runtime_close(&threaded_runtime)==0);C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);
 printf("PASS SCQ_DISPATCH checks=%u ACTUAL_DISPATCH_AND_PROBE REAL_FOREIGN_THREAD MODELED_NATIVE_APIS NO_APP_AUTHORITY bytes=%zu\n",checks,sizeof(d));return 0;
}

#endif
