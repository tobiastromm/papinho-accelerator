#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#include <wchar.h>
#include <conio.h>
#include "n0-protected-root-core.h"
#include "n0-protected-root-descriptor.h"

/* Fixed three-directory preparation ONLY. No files, SCM, ACL setters or deletes. */
static const WCHAR *paths[]={L"C:\\",L"C:\\ProgramData",L"C:\\ProgramData\\PapinhoAccelerator",L"C:\\ProgramData\\PapinhoAccelerator\\Protected",L"C:\\ProgramData\\PapinhoAccelerator\\Protected\\N0"};
static const char *parent_sha[]={"9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8","c0811643edc9703aa06642dfe053b7a0c3062cbaba03b6b169bde6962248f6a6"};
static const WCHAR confirmation[]=L"CREATE_N0_ROOT N0R-20261024-01 75330d11cc7e4123ae62eadc9a59c2f5";
typedef struct N0_NATIVE {
 HANDLE directory[5],extra[3],token,probe,thread_token;
 BY_HANDLE_FILE_INFORMATION identity[5];TOKEN_STATISTICS statistics;
 PSECURITY_DESCRIPTOR sd;PSID operator_sid;DWORD retained,close_error,output_error;
 BYTE observed_sd[5][32];unsigned observed_known[5];
} N0_NATIVE;
static N0_NATIVE state;
static volatile LONG stopped;
static DWORD error(void){DWORD e=GetLastError();return e?e:ERROR_INVALID_DATA;}
static BOOL WINAPI control(DWORD event){(void)event;InterlockedExchange(&stopped,1);return TRUE;}
static int stop(void *v){(void)v;if(_kbhit()&&_getwch()==27)InterlockedExchange(&stopped,1);return InterlockedCompareExchange(&stopped,0,0)!=0;}
static int same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow;}
static DWORD facts(HANDLE h,const WCHAR *path,BY_HANDLE_FILE_INFORMATION *id)
{
 WCHAR name[1024];DWORD n,flags;
 if(!GetFileInformationByHandle(h,id)||!GetHandleInformation(h,&flags))return error();
 if((flags&HANDLE_FLAG_INHERIT)||GetFileType(h)!=FILE_TYPE_DISK||(id->dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||!(id->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||id->dwVolumeSerialNumber!=417614100UL)return ERROR_INVALID_DATA;
 n=GetFinalPathNameByHandleW(h,name,1024,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
 if(!n)return error();
 if(n>=1024)return ERROR_INSUFFICIENT_BUFFER;
 if(n<4||wcsncmp(name,L"\\\\?\\",4)||_wcsicmp(name+4,path))return ERROR_INVALID_NAME;
 return 0;
}
static DWORD open_dir(unsigned index)
{
 HANDLE h=CreateFileW(paths[index],READ_CONTROL|FILE_READ_ATTRIBUTES|SYNCHRONIZE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(h==INVALID_HANDLE_VALUE)return error();state.directory[index]=h;
 return facts(h,paths[index],&state.identity[index]);
}
static DWORD close_ref(HANDLE *h)
{
 if(*h){if(!CloseHandle(*h)){state.retained=1;state.close_error=error();return state.close_error;}*h=NULL;}return 0;
}
static DWORD security(unsigned index)
{
 DWORD e,n,revision;SECURITY_DESCRIPTOR_CONTROL c;BYTE hash[32];unsigned j;
 if(state.sd)return ERROR_INVALID_STATE;
 e=GetSecurityInfo(state.directory[index],SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&state.sd);if(e)return e;
 n=GetSecurityDescriptorLength(state.sd);
 if(!IsValidSecurityDescriptor(state.sd)||!n||n>65536)e=ERROR_INVALID_SECURITY_DESCR;
 else if(!GetSecurityDescriptorControl(state.sd,&c,&revision))e=error();
 else if(index<2){if(!n0_hash_equal((BYTE *)state.sd,n,parent_sha[index]))e=ERROR_ACCESS_DENIED;}
 else {
  printf("N0_ROOT_SD index=%u bytes=%lu control=0x%04x; SACL_UNQUERIED\n",index,(unsigned long)n,(unsigned)c);
  if(!n0_sd_policy(state.sd,n))e=ERROR_ACCESS_DENIED;
  else if(!pacc_storage_hash_callback(NULL,(BYTE *)state.sd,n,hash))e=ERROR_INVALID_DATA;
  else if(state.observed_known[index]&&memcmp(hash,state.observed_sd[index],32))e=ERROR_ACCESS_DENIED;
  else {memcpy(state.observed_sd[index],hash,32);state.observed_known[index]=1;
   printf("N0_ROOT_OBSERVED_SD_SHA256 index=%u value=",index);for(j=0;j<32;j++)printf("%02x",hash[j]);puts("");}
 }
 if(ferror(stdout)&&!e)e=ERROR_WRITE_FAULT;
 if(LocalFree(state.sd)){state.retained=1;state.close_error=ERROR_INVALID_HANDLE;return e?e:state.close_error;}
 state.sd=NULL;return e;
}
static DWORD operator_current(int initial)
{
 BYTE user[256],admin[SECURITY_MAX_SID_SIZE];DWORD n,an=sizeof(admin);TOKEN_STATISTICS s;TOKEN_ELEVATION elevated;TOKEN_TYPE type;BOOL member;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&state.thread_token)){
  DWORD e=close_ref(&state.thread_token);return e?e:ERROR_ACCESS_DENIED;
 }
 if(GetLastError()!=ERROR_NO_TOKEN)return error();
 if(initial){if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&state.token))return error();}
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&state.probe))return error();
 if(!GetTokenInformation(state.probe,TokenStatistics,&s,sizeof(s),&n))return error();
 if(n!=sizeof(s))return ERROR_INVALID_DATA;
 if(!GetTokenInformation(state.probe,TokenUser,user,sizeof(user),&n))return error();
 if(!GetTokenInformation(state.probe,TokenElevation,&elevated,sizeof(elevated),&n))return error();
 if(!elevated.TokenIsElevated)return ERROR_ACCESS_DENIED;
 if(!GetTokenInformation(state.probe,TokenType,&type,sizeof(type),&n))return error();
 if(type!=TokenPrimary)return ERROR_ACCESS_DENIED;
 if(!state.operator_sid&&!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&state.operator_sid))return error();
 member=EqualSid(((TOKEN_USER *)user)->User.Sid,state.operator_sid);
 if(!member)return ERROR_ACCESS_DENIED;
 if(!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin,&an)||!CheckTokenMembership(NULL,admin,&member))return error();
 if(!member)return ERROR_ACCESS_DENIED;
 if(initial)state.statistics=s;
 else if(memcmp(&s.TokenId,&state.statistics.TokenId,sizeof(LUID))||memcmp(&s.AuthenticationId,&state.statistics.AuthenticationId,sizeof(LUID))||memcmp(&s.ModifiedId,&state.statistics.ModifiedId,sizeof(LUID)))return ERROR_ACCESS_DENIED;
 return close_ref(&state.probe);
}
static DWORD parent(void *v,unsigned i)
{
 unsigned j;DWORD e;BY_HANDLE_FILE_INFORMATION id;(void)v;
 if(state.output_error)return state.output_error;
 if((e=operator_current(0)))return e;
 for(j=0;j<i+2;j++){
  if((e=facts(state.directory[j],paths[j],&id))||!same(&id,&state.identity[j]))return e?e:ERROR_INVALID_DATA;
  if((e=security(j)))return e;
 }
 return 0;
}
static DWORD absent(void *v,unsigned i)
{
 DWORD a;(void)v;a=GetFileAttributesW(paths[i+2]);
 if(a!=INVALID_FILE_ATTRIBUTES)return ERROR_ALREADY_EXISTS;
 a=GetLastError();return a==ERROR_FILE_NOT_FOUND?0:a;
}
static DWORD create(void *v,unsigned i)
{
 SECURITY_ATTRIBUTES sa;DWORD e;(void)v;
 /* Revalidate immediately before effect; never adopt an already-existing name. */
 if((e=parent(v,i)))return e;if(stop(v))return ERROR_CANCELLED;
 sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=(PSECURITY_DESCRIPTOR)n0_root_descriptor;sa.bInheritHandle=FALSE;
 if(printf("N0_ROOT_CREATE_BEGIN index=%u path=%ls; ORIGINAL104_ONLY\n",i,paths[i+2])<0||fflush(stdout))return ERROR_WRITE_FAULT;
 if(!CreateDirectoryW(paths[i+2],&sa)){
  e=error();printf("N0_ROOT_CREATE_FAILED index=%u win32=%lu; NO_ADOPTION_NO_RETRY_NO_DELETE; FALSE_IS_NOT_A_CLEANUP_RECEIPT\n",i,(unsigned long)e);fflush(stdout);return e;
 }
 if(printf("N0_ROOT_CREATED index=%u; PRESERVE_ON_FAILURE_NO_DELETE\n",i)<0||fflush(stdout))state.output_error=ERROR_WRITE_FAULT;
 return 0;
}
static DWORD open_created(void *v,unsigned i){(void)v;return state.output_error?state.output_error:open_dir(i+2);}
static DWORD verify(void *v,unsigned i)
{
 BY_HANDLE_FILE_INFORMATION id;DWORD e;(void)v;
 if((e=facts(state.directory[i+2],paths[i+2],&id))||!same(&id,&state.identity[i+2]))return e?e:ERROR_INVALID_DATA;
 if((e=security(i+2)))return e;
 if(printf("N0_ROOT_VERIFIED index=%u native=%lu:%lu:%lu; OWNER_GROUP=BA; CONTROL=0x9004; SYSTEM_THEN_BA_FA_OICI\n",i,(unsigned long)id.dwVolumeSerialNumber,(unsigned long)id.nFileIndexHigh,(unsigned long)id.nFileIndexLow)<0||fflush(stdout))return ERROR_WRITE_FAULT;
 return parent(v,i);
}
static DWORD finish(void *v,unsigned i)
{
 DWORD e;(void)v;
 if(!DuplicateHandle(GetCurrentProcess(),state.directory[i+2],GetCurrentProcess(),&state.extra[i],0,FALSE,DUPLICATE_SAME_ACCESS))return error();
 e=close_ref(&state.directory[i+2]);if(e)return e;
 state.directory[i+2]=state.extra[i];state.extra[i]=NULL;
 return printf("N0_ROOT_ACQUISITION_CLOSED index=%u; SAME_OBJECT_PARENT_REFERENCE_RETAINED\n",i)<0?ERROR_WRITE_FAULT:0;
}
static DWORD close_all(void *v)
{
 int i;DWORD e;(void)v;if(state.retained||state.sd){state.retained=1;return state.close_error?state.close_error:ERROR_INVALID_STATE;}
 for(i=2;i>=0;i--)if((e=close_ref(&state.extra[i])))return e;
 for(i=4;i>=0;i--)if((e=close_ref(&state.directory[i])))return e;
 if((e=close_ref(&state.probe))||(e=close_ref(&state.thread_token))||(e=close_ref(&state.token)))return e;
 if(state.operator_sid){if(LocalFree(state.operator_sid)){state.retained=1;state.close_error=ERROR_INVALID_HANDLE;return state.close_error;}state.operator_sid=NULL;}
 return 0;
}
static DWORD prepare(void)
{
 DWORD e,serial,flags,component;WCHAR fs[32];unsigned i;
 if(!n0_sd_input(n0_root_descriptor,sizeof(n0_root_descriptor)))return ERROR_INVALID_DATA;
 if((e=operator_current(1)))return e;
 if(!GetVolumeInformationW(L"C:\\",NULL,0,&serial,&component,&flags,fs,32))return error();
 if(serial!=417614100UL||!(flags&FS_PERSISTENT_ACLS)||wcscmp(fs,L"NTFS"))return ERROR_INVALID_DATA;
 for(i=0;i<2;i++){
  if((e=open_dir(i)))return e;
  if(state.identity[i].nFileIndexHigh!=(i?131072UL:327680UL)||state.identity[i].nFileIndexLow!=(i?227641UL:5UL))return ERROR_INVALID_DATA;
  if((e=security(i)))return e;
 }
 /* Root absence is mandatory; descendants may return PATH_NOT_FOUND while root is absent. */
 return absent(NULL,0);
}
int wmain(int argc,WCHAR **argv)
{
 N0_ROOT_FLOW flow;N0_ROOT_OPS ops;DWORD e,closed,mode;WCHAR input[200];int handler=0;unsigned i;
 ZeroMemory(&state,sizeof(state));ZeroMemory(&flow,sizeof(flow));
 if(argc==2&&!wcscmp(argv[1],L"--describe")){
  if(!n0_sd_input(n0_root_descriptor,sizeof(n0_root_descriptor)))return 2;
  printf("N0_ROOT_DESCRIPTOR=VALID; BYTES=104; SHA256=688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579; CONTROL=0x9004; OWNER_GROUP=BA; SYSTEM_THEN_BA_FA_OICI; STATE_BYTES=%zu; MEMORY_ONLY_NO_CREATE\n",sizeof(state));return 0;
 }
 if(argc!=2||wcscmp(argv[1],L"--create"))return 2;
 /* Confirmation/Stop use inherited console input; stdout may be the pinned
  * launcher's bounded receipt relay. Output never grants authority. */
 if(!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode))return 2;
 if(!SetConsoleCtrlHandler(control,TRUE))return 2;handler=1;
 e=prepare();
 if(!e){
  puts("N0R-20261024-01: THREE NEW DIRECTORIES ONLY. No files/ACL setters/SCM/Start. Existing root => STOP.");
  puts("DESCRIPTOR=104 ORIGINAL BYTES; SHA256=688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579; CONTROL=0x9004; NO_SDDL_REGENERATION");
  for(i=2;i<5;i++)printf("CREATE_ONLY=%ls\n",paths[i]);
  printf("Type exactly: %ls\n",confirmation);fflush(stdout);
  if(!fgetws(input,200,stdin))e=ERROR_OPERATION_ABORTED;
  else{size_t n=wcslen(input);while(n&&(input[n-1]==L'\n'||input[n-1]==L'\r'))input[--n]=0;if(wcscmp(input,confirmation))e=ERROR_ACCESS_DENIED;}
 }
 if(!e){
  ops.context=&state;ops.stop=stop;ops.parent=parent;ops.absent=absent;ops.create=create;ops.open=open_created;ops.verify=verify;ops.finish=finish;ops.close=close_all;
  e=n0_root_run(&flow,&ops);closed=flow.close_error;
 }else closed=close_all(NULL);
 if(handler&&!state.retained&&!state.sd){
  if(!SetConsoleCtrlHandler(control,FALSE)){DWORD h=error();if(!e){e=h;flow.stage="CONTROL_HANDLER_CLOSE";}printf("N0_ROOT_CONTROL_CLOSE_ERROR=%lu\n",(unsigned long)h);}
  handler=0;
 }
 if(printf("N0_ROOT_FINAL first=%lu stage=%s close=%lu created=%u verified=%u acquisition_closed=%u retained=%lu; ED11=PARTIAL; AUTHORITY=DENY; NO_RETRY_NO_DELETE\n",(unsigned long)e,flow.stage?flow.stage:"PREPARE_OR_CONFIRM",(unsigned long)closed,flow.created,flow.verified,flow.finished,(unsigned long)state.retained)<0||fflush(stdout)){if(!e)e=ERROR_WRITE_FAULT;}
 if(state.retained||state.sd){puts("N0_ROOT_RETAINED_UNKNOWN: preserve this original process, no forced close/retry.");fflush(stdout);for(;;)Sleep(1000);}
 return e||closed?2:0;
}
