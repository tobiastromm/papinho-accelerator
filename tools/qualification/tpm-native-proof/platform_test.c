#include "host_platform.h"
#include <stdio.h>
#include <sddl.h>
#include <limits.h>
#include <string.h>
#include <wchar.h>
static unsigned checks;
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u %s\n",(unsigned)__LINE__,#x);return 0;}}while(0)
static struct {int active,mode,reentered;TP_PLATFORM *owner;int file_fault;DWORD own,parent_count,console_count,stats_calls;HANDLE parent,console;DWORD opens,closes,class_calls,client_calls;} g;
static BOOL q_count(HANDLE h,PDWORD count){if(!g.active)return GetProcessHandleCount(h,count);if(h==GetCurrentProcess()){*count=g.own;if(g.mode==1){SetLastError(5);return FALSE;}return TRUE;}*count=h==g.parent?g.parent_count:g.console_count;return TRUE;}
static LONG NTAPI q_basic(HANDLE h,ULONG cls,PVOID data,ULONG bytes,PULONG used){ULONGLONG pid=100;
 if(cls==49){g.class_calls++;if(g.mode==26&&!g.reentered){g.reentered=1;hp_admin(g.owner);}if(bytes!=8)return -1;pid=201;if(g.mode==17)return (LONG)0xc0000003;if(g.mode==18&&h==g.parent)pid=301;if(g.mode==20)pid=202;if(g.mode==22&&g.class_calls>2)pid=205;memcpy(data,&pid,8);*used=g.mode==21?4:8;return 0;}
 if(bytes!=48)return -1;memset(data,0,48);memcpy((unsigned char *)data+40,&pid,8);*used=48;return 0;}
static DWORD q_id(HANDLE h){if(!g.active)return GetProcessId(h);return h==g.parent?100:200;}
static DWORD q_wait(HANDLE h,DWORD time){if(!g.active)return WaitForSingleObject(h,time);return g.mode==19&&h==g.console?WAIT_OBJECT_0:WAIT_TIMEOUT;}
static BOOL q_session(DWORD pid,DWORD *session){if(!g.active)return ProcessIdToSessionId(pid,session);*session=1;return TRUE;}
static int model_admin_evidence(TP_PLATFORM *,HANDLE,unsigned);
static int model_abi_evidence(TP_PLATFORM *);

static FARPROC q_proc(HMODULE m,LPCSTR name){if(g.active&&!strcmp(name,"NtQueryInformationProcess"))return (FARPROC)(void *)q_basic;return GetProcAddress(m,name);}
static HWND q_console(void){return g.active?(HWND)(ULONG_PTR)1:GetConsoleWindow();}
static DWORD q_window(HWND window,LPDWORD pid){if(!g.active)return GetWindowThreadProcessId(window,pid);*pid=g.mode==8?100:g.mode==9?GetCurrentProcessId():200;return 1;}
static DWORD q_clients(LPDWORD p,DWORD n){if(!g.active)return GetConsoleProcessList(p,n);g.client_calls++;if(n<3)return 0;p[0]=GetCurrentProcessId();p[1]=100;p[2]=300;return g.mode==4||(g.mode==23&&g.client_calls>1)?3:2;}
static HANDLE q_open(DWORD access,BOOL inherit,DWORD pid){HANDLE h;if(!g.active)return OpenProcess(access,inherit,pid);g.opens++;if(g.mode==5&&pid==200){SetLastError(5);return NULL;}h=CreateEventW(NULL,TRUE,FALSE,NULL);if(pid==100)g.parent=h;else g.console=h;return h;}
static BOOL q_times(HANDLE h,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u){if(!g.active)return GetProcessTimes(h,c,e,k,u);memset(c,0,sizeof(*c));memset(e,0,sizeof(*e));memset(k,0,sizeof(*k));memset(u,0,sizeof(*u));c->dwLowDateTime=h==g.parent?10:20;if(g.mode==6)c->dwLowDateTime++;return TRUE;}
static BOOL q_close(HANDLE h){if(!g.active){if(g.file_fault==6&&GetFileType(h)==FILE_TYPE_DISK){SetLastError(6);return FALSE;}return CloseHandle(h);}g.closes++;if(g.mode==7&&h==g.console){SetLastError(6);return FALSE;}return CloseHandle(h);}
static BOOL q_thread(HANDLE thread,DWORD access,BOOL asSelf,PHANDLE token){if(!g.active)return OpenThreadToken(thread,access,asSelf,token);*token=NULL;if(g.mode==10){*token=CreateEventW(NULL,TRUE,FALSE,NULL);return TRUE;}SetLastError(g.mode==11?ERROR_ACCESS_DENIED:ERROR_NO_TOKEN);return FALSE;}
static BOOL q_token(HANDLE process,DWORD access,PHANDLE token){if(!g.active)return OpenProcessToken(process,access,token);*token=CreateEventW(NULL,TRUE,FALSE,NULL);return *token!=NULL;}
static BOOL q_information(HANDLE token,TOKEN_INFORMATION_CLASS cls,LPVOID data,DWORD n,PDWORD used){static const unsigned char sid[]={1,5,0,0,0,0,0,5,21,0,0,0,222,210,212,46,186,166,219,85,236,129,65,100,84,4,0,0};if(!g.active)return GetTokenInformation(token,cls,data,n,used);
 if(g.mode==15){SetLastError(5);return FALSE;}
 if(cls==TokenUser){TOKEN_USER *v=data;if(n<sizeof(*v)+sizeof(sid))return FALSE;*used=(DWORD)(sizeof(*v)+sizeof(sid));memset(data,0,*used);v->User.Sid=(unsigned char *)data+sizeof(*v);memcpy(v->User.Sid,sid,sizeof(sid));if(g.mode==12)((unsigned char *)v->User.Sid)[24]^=1;return TRUE;}
 if(cls==TokenElevation){TOKEN_ELEVATION *v=data;v->TokenIsElevated=g.mode!=13;*used=sizeof(*v);return TRUE;}
 if(cls==TokenType){*(TOKEN_TYPE *)data=g.mode==29?TokenImpersonation:TokenPrimary;*used=sizeof(TOKEN_TYPE);return TRUE;}
 if(cls==TokenSessionId){*(DWORD *)data=g.mode==30?99:1;*used=sizeof(DWORD);return TRUE;}
 if(cls==TokenStatistics){TOKEN_STATISTICS *v=data;if(n<sizeof(*v))return FALSE;memset(v,0,sizeof(*v));v->TokenId.LowPart=111;v->AuthenticationId.LowPart=222;v->ModifiedId.LowPart=333+(g.mode==14?(g.stats_calls%2):0)+(g.mode==16?1:0);*used=sizeof(*v);g.stats_calls++;return TRUE;}return FALSE;
}
static BOOL q_membership(HANDLE token,PSID sid,PBOOL enabled){if(!g.active)return CheckTokenMembership(token,sid,enabled);*enabled=g.mode!=13;return TRUE;}
static BOOL q_security(HANDLE h,SECURITY_INFORMATION flags,PSECURITY_DESCRIPTOR data,DWORD bytes,PDWORD needed){BOOL ok;
 if(g.file_fault==1){SetLastError(5);return FALSE;}ok=GetKernelObjectSecurity(h,flags,data,bytes,needed);if(ok&&g.file_fault==2&&*needed)((unsigned char *)data)[4]^=1;return ok;}
static BOOL q_file_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION info){BOOL ok=GetFileInformationByHandle(h,info);if(ok&&g.file_fault==3)info->nFileIndexLow^=1;return ok;}
static BOOL q_read(HANDLE h,LPVOID b,DWORD n,LPDWORD used,LPOVERLAPPED overlapped){if(g.file_fault==4){*used=0;SetLastError(999);return TRUE;}return ReadFile(h,b,n,used,overlapped);}
static NTSTATUS q_finish_hash(BCRYPT_HASH_HANDLE h,PUCHAR b,ULONG n,ULONG flags){NTSTATUS status=BCryptFinishHash(h,b,n,flags);if(status==0&&g.file_fault==5&&n)b[0]^=1;return status;}
#define GetKernelObjectSecurity q_security
#define GetFileInformationByHandle q_file_info
#define ReadFile q_read
#define BCryptFinishHash q_finish_hash
#define GetProcessHandleCount q_count
#define GetProcAddress q_proc
#define GetConsoleWindow q_console
#define GetWindowThreadProcessId q_window
#define GetConsoleProcessList q_clients
#define OpenProcess q_open
#define GetProcessTimes q_times
#define GetProcessId q_id
#define WaitForSingleObject q_wait
#define ProcessIdToSessionId q_session
#define HP_ADMIN_EVIDENCE model_admin_evidence
#define HP_ABI_EVIDENCE model_abi_evidence
#define CloseHandle q_close
#define OpenThreadToken q_thread
#define OpenProcessToken q_token
#define GetTokenInformation q_information
#define CheckTokenMembership q_membership
/* Exact production function bodies; only OS calls above are modelled. */
#include "host_platform.c"
static int model_admin_evidence(TP_PLATFORM *p,HANDLE h,unsigned role){if(!g.active)return hp_admin_evidence(p,h,role);return g.mode==24?tp_fail(&p->proof,"MODEL_ROLE_EVIDENCE_DENY",0):1;}
static int model_abi_evidence(TP_PLATFORM *p){return g.active?1:hp_role_file(p,NULL,2);}
static TP_PLATFORM platform,retained,role_file_retained;

static TJ_DOC doc;
static int parse(const char *s){size_t n=strlen(s);if(n>=TJ_BYTES)return 0;memcpy(doc.bytes,s,n);return tj_parse(&doc,n);}
static DWORD WINAPI other_admin_thread(void *p){return hp_admin(p)?1:0;}
static int test(int metadata){unsigned mode,i;ULONGLONG a,b;unsigned long long n;char out[80];PSID expected=NULL;
 C(ConvertStringSidToSidA(TP_OPERATOR,&expected));C(EqualSid(expected,(PSID)operator_sid));C(LocalFree(expected)==NULL);
 C(hp_utc_filetime(L"2026-10-26T21:35:16.5000000Z",&a));C(hp_utc_filetime(L"2026-10-26T21:35:16.5Z",&b)&&a==b);C(hp_utc_filetime(L"2026-10-26T18:35:16.5-03:00",&b)&&a==b);
 C(!hp_utc_filetime(L"2026-10-26T21:35:16.50000001Z",&b));C(!hp_utc_filetime(L"2026-13-26T21:35:16Z",&b));C(!hp_utc_filetime(L"2026-10-26T21:35:16Zjunk",&b));
 C(parse("{\"a\":true,\"n\":18446744073709551615,\"nested\":{\"v\":\"ok\"}}"));C(tj_true(&doc,tj_get(&doc,0,"a")));C(tj_u64(&doc,tj_get(&doc,0,"n"),&n)&&n==ULLONG_MAX);C(tj_string(&doc,tj_get(&doc,tj_get(&doc,0,"nested"),"v"),out,sizeof(out))&&!strcmp(out,"ok"));
 {const char *bad[]={"{\"a\":1,\"a\":2}","{\"a\":1,}","{\"a\":01}","{\"a\":-1}","{\"a\":1.0}","{\"\\u0061\":1}","{}junk","{\"a\":tru}","{\"a\":\"x\\q\"}","[1,2]","{\"a\":[1,]}","{\"a\":\"unfinished}"};for(i=0;i<sizeof(bad)/sizeof(bad[0]);i++)C(!parse(bad[i]));}
 C(parse("{\"n\":18446744073709551616}"));C(!tj_u64(&doc,tj_get(&doc,0,"n"),&n));
 memset(&g,0,sizeof(g));g.active=1;g.own=124;g.parent_count=731;g.console_count=271;
 for(mode=0;mode<=7;mode++){
 TP_PLATFORM *p=mode==7?&retained:&platform;hp_init(p);g.mode=(int)mode;g.own=mode==2?125:124;g.parent_count=mode==3?754:731;g.opens=0;g.closes=0;g.class_calls=g.client_calls=0;
 if(mode==6){g.mode=0;C(hp_admin(p));g.mode=6;C(!hp_admin(p)&&!strcmp(p->proof.first,"ADMIN_PID_REUSE"));}
 else if(mode==0){C(hp_admin(p)&&p->admin_count==1&&!p->proof.controlled_refs);{unsigned closes=g.closes;C(hp_close(p)&&hp_close(p)&&g.closes==closes);}}
 else C(!hp_admin(p));
 if(mode==1)C(!strcmp(p->proof.first,"NATIVE_OBSERVER_QUERY_FAILED")&&p->observer_error==5&&!g.opens);
 if(mode==2)C(!strcmp(p->proof.first,"NATIVE_OBSERVER_HEADROOM")&&p->observer_count==125&&!g.opens);
 if(mode==3)C(!strcmp(p->proof.first,"ADMIN_BUDGET_DENY"));if(mode==4)C(!strcmp(p->proof.first,"PARTICIPANT_TOPOLOGY_DENY")&&!g.opens);
 if(mode==5)C(!strcmp(p->proof.first,"CONSOLE_OPEN_QUERY")&&!p->parent&&!p->console);
 if(mode==7){unsigned opens=g.opens,closes=g.closes;C(!hp_close(p)&&g.closes==closes);C(p->proof.retained&&p->console&&p->parent);C(!hp_admin(p)&&g.opens==opens&&g.closes==closes);}
 }
 /* C0-C9 aliases are metadata; actual auxiliary remains distinct and counted. */
 for(mode=8;mode<=9;mode++){
 hp_init(&platform);g.mode=(int)mode;g.own=94;g.opens=g.closes=g.class_calls=g.client_calls=0;
 C(hp_admin(&platform)&&platform.topology_console==200&&platform.parent_pid==100);
 C(platform.window_owner==(mode==8?100:GetCurrentProcessId())&&platform.admin_count==1&&!platform.proof.controlled_refs);
 }
 for(mode=17;mode<=24;mode++){
 hp_init(&platform);g.mode=(int)mode;g.own=94;g.opens=g.closes=g.class_calls=g.client_calls=0;
 C(!hp_admin(&platform)&&platform.proof.frozen&&!platform.proof.retained&&!platform.proof.controlled_refs);
 C(!hp_admin(&platform));
 }
 hp_init(&platform);g.mode=0;g.own=94;g.opens=g.closes=0;platform.admin_busy=1;C(!hp_admin(&platform)&&!strcmp(platform.proof.first,"ADMIN_REENTRANT")&&!g.opens);
 hp_init(&platform);g.mode=26;g.owner=&platform;g.reentered=0;g.opens=g.closes=0;C(!hp_admin(&platform)&&g.reentered&&!g.opens&&!strcmp(platform.proof.first,"ADMIN_REENTRANT"));
 hp_init(&platform);g.mode=0;g.opens=g.closes=0;{HANDLE thread;g.active=0;thread=CreateThread(NULL,0,other_admin_thread,&platform,0,NULL);DWORD code;C(thread!=NULL);C(q_wait(thread,5000)==WAIT_OBJECT_0);C(GetExitCodeThread(thread,&code)&&code==0);C(q_close(thread));C(!g.opens&&!g.closes&&!strcmp(platform.proof.first,"NONE"));g.active=1;}
 puts("PASS C0_NATIVE_DISCOVERY_ALIASES_QUERY_LIFETIME_REVALIDATION_NEGATIVES");
 for(mode=0;mode<=6;mode++){hp_init(&platform);g.mode=mode?9+(int)mode:0;g.stats_calls=0;
 if(!mode){C(hp_token(&platform));g.mode=16;C(!hp_token(&platform)&&!strcmp(platform.proof.first,"TOKEN_CONTINUITY_DENY"));}
 else C(!hp_token(&platform));C(!platform.token&&!platform.proof.controlled_refs);
 }
 for(mode=0;mode<7;mode++){
 hp_init(&platform);g.mode=mode==0?0:mode==1?12:mode==2?13:mode==3?14:mode==4?15:mode==5?29:30;g.stats_calls=0;
 platform.token_bound=1;{LUID auth={222,0};memcpy(platform.token_ids+8,&auth,8);}
 if(!mode)C(hp_role_token(&platform,(HANDLE)(ULONG_PTR)123,0)&&platform.role_token_bound[0]);else C(!hp_role_token(&platform,(HANDLE)(ULONG_PTR)123,0)&&platform.proof.frozen);
 C(!platform.token&&!platform.proof.controlled_refs);
 }
 if(metadata==2){
 g.active=0;g.file_fault=0;hp_init(&platform);C(tp_crypto_init(&platform.proof));for(i=0;i<3;i++)C(hp_role_file(&platform,NULL,i));C(tp_finish(&platform.proof));C(hp_close(&platform));
 for(i=1;i<=6;i++){TP_PLATFORM *p=i==6?&role_file_retained:&platform;hp_init(p);C(tp_crypto_init(&p->proof));g.file_fault=(int)i;p->proof.stage="ROLE_FILE_NEGATIVE";
 C(!hp_role_file(p,NULL,1)&&p->proof.frozen);if(i==4)C(!strcmp(p->proof.first,"ROLE_IMAGE_SHORT_READ")&&!p->proof.first_native);
 if(i==6){C(p->proof.retained&&p->role_image);C(!hp_role_file(p,NULL,1)&&!hp_close(p));}else{g.file_fault=0;C(tp_finish(&p->proof));C(hp_close(p));C(!p->role_image&&!p->proof.controlled_refs);}}
 puts("PASS REAL_WINDOWS_ROLE_FILES_NATIVE_ID_HASH_DESCRIPTOR_AND_SIX_FAULTS_NO_TPM");
 }
 if(metadata==1){g.active=0;hp_init(&platform);C(tp_crypto_init(&platform.proof));C(hp_boot(&platform));C(tp_finish(&platform.proof));C(hp_close(&platform));}
 printf("PASS PLATFORM assertions=%u real_metadata_opt_in_only no_TPM no_approval owner_bytes=%Iu\n",checks,sizeof(TP_PLATFORM));return 1;
}
int main(int argc,char **argv){return test(argc==2&&!strcmp(argv[1],"--current-metadata-readonly")?1:argc==2&&!strcmp(argv[1],"--role-files-readonly")?2:0)?0:1;}
