#define _WIN32_WINNT 0x0A00
#include <windows.h>
#include <winternl.h>
#include <psapi.h>
#include <sddl.h>
#include <bcrypt.h>
#include <wintrust.h>
#include <softpub.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#define CAP 8
#define R_SEED 1U
#define R_OBSERVER 2U
#define R_WINDOW 4U
#define R_NATIVE_HOST 8U
#define R_CLIENT 16U
#define R_ANCESTOR 32U
#define R_BASELINE_HOST 64U
typedef LONG (NTAPI *QUERY)(HANDLE,ULONG,PVOID,ULONG,PULONG);
typedef struct NODE {
 HANDLE process;DWORD pid,parent,roles,session,error,token_error,metric_error,count,exit_code,wait_status;
 ULONGLONG creation,after_creation,host_raw,host_after,tick;
 LONG host_status,host_after_status;ULONG host_bytes,host_after_bytes;
 int after_queried,identity,stable,token_known,elevated,metrics,image_known,hash_known;SIZE_T private_bytes,ws;
 wchar_t image[1024],sid[192];BY_HANDLE_FILE_INFORMATION file;char hash[65];LONG file_trust;
} NODE;
typedef struct OBS {
 NODE nodes[CAP];unsigned n;QUERY query;HANDLE temporary;int reserved,frozen,retained;
 int test_close_failure;unsigned close_calls;const char *first;DWORD first_error,own_peak,own_last,close_error;BCRYPT_ALG_HANDLE sha;BCRYPT_HASH_HANDLE hash;
 unsigned char object[4096],block[4096];DWORD ids[16],members,window_pid,attach_error,before_clients,after_detach_clients,detach_error;int attached,detach_ok;
} OBS;
static OBS observation;
static volatile LONG stopped;
static ULONGLONG filetime(FILETIME f){return ((ULONGLONG)f.dwHighDateTime<<32)|f.dwLowDateTime;}
static int fail(OBS *o,const char *first,DWORD code){if(!o->frozen){o->first=first;o->first_error=code;}o->frozen=1;return 0;}
static BOOL WINAPI stop_handler(DWORD code){(void)code;InterlockedExchange(&stopped,1);return TRUE;}
static int sample(OBS *o,int reserve){DWORD count=0;if(o->frozen)return 0;if(InterlockedCompareExchange(&stopped,0,0))return fail(o,"STOP",0);
 if(!GetProcessHandleCount(GetCurrentProcess(),&count))return fail(o,"OWN_COUNT_QUERY",GetLastError());o->own_last=count;if(count>o->own_peak)o->own_peak=count;
 if(count>(reserve?124U:128U))return fail(o,"OWN_BUDGET",0);return 1;}
static int close_ref(OBS *o,HANDLE *h){if(!*h||*h==INVALID_HANDLE_VALUE)return 1;if(o->retained)return 0;
 o->close_calls++;if(o->test_close_failure||!CloseHandle(*h)){o->close_error=o->test_close_failure?ERROR_GEN_FAILURE:GetLastError();o->retained=1;return fail(o,"CLOSE_UNKNOWN",o->close_error);}*h=NULL;return 1;}
static void init(OBS *o){memset(o,0,sizeof(*o));o->first="NONE";o->query=(QUERY)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtQueryInformationProcess");}
static void json_wide(FILE *f,const wchar_t *s){char b[4096];unsigned i;int n=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,s,-1,b,sizeof(b),NULL,NULL);if(n<1){fputs("null",f);return;}fputc('"',f);for(i=0;i<(unsigned)n-1;i++){unsigned char c=(unsigned char)b[i];if(c=='"'||c=='\\')fputc('\\',f);if(c<32)fprintf(f,"\\u%04x",c);else fputc(c,f);}fputc('"',f);}
static int reserve_temp(OBS *o){if(o->reserved||o->temporary)return fail(o,"TEMP_BUSY",0);if(!sample(o,1))return 0;o->reserved=1;return 1;}
static int end_temp(OBS *o){if(!close_ref(o,&o->temporary))return 0;o->reserved=0;return !o->frozen;}
static void token(OBS *o,NODE *n){unsigned char data[4096];DWORD bytes=0;TOKEN_USER *user=(TOKEN_USER *)data;TOKEN_ELEVATION elevated;LPWSTR sid=NULL;
 if(!reserve_temp(o))return;if(!OpenProcessToken(n->process,TOKEN_QUERY,&o->temporary)){n->token_error=GetLastError();o->reserved=0;return;}
 if(GetTokenInformation(o->temporary,TokenUser,data,sizeof(data),&bytes)&&bytes>=sizeof(TOKEN_USER)&&bytes<=sizeof(data)&&
 (ULONG_PTR)user->User.Sid>=(ULONG_PTR)data&&(ULONG_PTR)user->User.Sid<=(ULONG_PTR)data+bytes-8&&IsValidSid(user->User.Sid)&&
 GetLengthSid(user->User.Sid)<=bytes-(DWORD)((ULONG_PTR)user->User.Sid-(ULONG_PTR)data)&&ConvertSidToStringSidW(user->User.Sid,&sid)){
 wcscpy_s(n->sid,192,sid);if(LocalFree(sid)){o->retained=1;fail(o,"SID_LOCALFREE_UNKNOWN",GetLastError());return;}
 if(GetTokenInformation(o->temporary,TokenElevation,&elevated,sizeof(elevated),&bytes)&&bytes==sizeof(elevated)){n->elevated=elevated.TokenIsElevated!=0;n->token_known=1;}else n->token_error=GetLastError();
 }else n->token_error=GetLastError();end_temp(o);
}
static void image(OBS *o,NODE *n){DWORD size=1024,used=0,length=0,got=0;NTSTATUS status;unsigned char digest[32];BY_HANDLE_FILE_INFORMATION after;LARGE_INTEGER total;ULONGLONG bytes=0;unsigned i;
 if(!QueryFullProcessImageNameW(n->process,0,n->image,&size)){n->error=GetLastError();return;}if(!reserve_temp(o))return;
 o->temporary=CreateFileW(n->image,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(o->temporary==INVALID_HANDLE_VALUE){n->error=GetLastError();o->temporary=NULL;o->reserved=0;return;}
 if(!GetFileInformationByHandle(o->temporary,&n->file)||!GetFileSizeEx(o->temporary,&total)){n->error=GetLastError();goto done;}
 if(n->file.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT||total.QuadPart<1||total.QuadPart>67108864){n->error=ERROR_INVALID_DATA;goto done;}n->image_known=1;
 if(!o->sha){status=BCryptOpenAlgorithmProvider(&o->sha,BCRYPT_SHA256_ALGORITHM,NULL,0);if(status!=0||!o->sha){if(o->sha)o->retained=1;fail(o,"SHA_OPEN",(DWORD)status);goto done;}}
 status=BCryptGetProperty(o->sha,BCRYPT_OBJECT_LENGTH,(PUCHAR)&length,sizeof(length),&used,0);if(status!=0||used!=sizeof(length)||length>sizeof(o->object)){fail(o,"SHA_SIZE",(DWORD)status);goto done;}
 status=BCryptCreateHash(o->sha,&o->hash,o->object,length,NULL,0,0);if(status!=0||!o->hash){if(o->hash)o->retained=1;fail(o,"HASH_CREATE",(DWORD)status);goto done;}
 for(;;){if(InterlockedCompareExchange(&stopped,0,0)){fail(o,"STOP",0);break;}
 if(!ReadFile(o->temporary,o->block,sizeof(o->block),&got,NULL)){fail(o,"IMAGE_READ",GetLastError());break;}if(!got)break;bytes+=got;if(bytes>67108864){fail(o,"IMAGE_GROWTH",0);break;}
 status=BCryptHashData(o->hash,o->block,got,0);if(status!=0){fail(o,"HASH_DATA",(DWORD)status);break;}}
 if(!o->frozen){status=BCryptFinishHash(o->hash,digest,32,0);if(status!=0)fail(o,"HASH_FINISH",(DWORD)status);}
 status=BCryptDestroyHash(o->hash);if(status!=0){o->retained=1;fail(o,"HASH_CLOSE_UNKNOWN",(DWORD)status);return;}o->hash=NULL;
 if(!o->frozen&&bytes==(ULONGLONG)total.QuadPart&&GetFileInformationByHandle(o->temporary,&after)&&after.dwVolumeSerialNumber==n->file.dwVolumeSerialNumber&&after.nFileIndexHigh==n->file.nFileIndexHigh&&after.nFileIndexLow==n->file.nFileIndexLow&&after.nFileSizeHigh==n->file.nFileSizeHigh&&after.nFileSizeLow==n->file.nFileSizeLow){static const char hex[]="0123456789abcdef";for(i=0;i<32;i++){n->hash[2*i]=hex[digest[i]>>4];n->hash[2*i+1]=hex[digest[i]&15];}n->hash[64]=0;n->hash_known=1;}
 /* Authenticode validation deliberately separate: no authority from metadata. */
 done:end_temp(o);
}
static void metrics(OBS *o,NODE *n){PROCESS_MEMORY_COUNTERS_EX m;DWORD exit_code=0;FILETIME c,e,k,u;
 if(!sample(o,0))return;n->tick=GetTickCount64();n->metrics=GetProcessHandleCount(n->process,&n->count)!=0;n->metric_error=n->metrics?0:GetLastError();
 memset(&m,0,sizeof(m));m.cb=sizeof(m);if(!GetProcessMemoryInfo(n->process,(PROCESS_MEMORY_COUNTERS *)&m,sizeof(m))){n->metrics=0;n->metric_error=GetLastError();}else{n->private_bytes=m.PrivateUsage;n->ws=m.WorkingSetSize;}
 if(GetProcessTimes(n->process,&c,&e,&k,&u)&&GetExitCodeProcess(n->process,&exit_code)){n->after_creation=filetime(c);n->exit_code=exit_code;n->wait_status=WaitForSingleObject(n->process,0);n->stable=n->creation==n->after_creation&&exit_code==STILL_ACTIVE&&n->wait_status==WAIT_TIMEOUT;}else{n->stable=0;n->error=GetLastError();}
}
static int bind_role(OBS *o,DWORD pid,DWORD role){NODE *n;unsigned i;FILETIME c,e,k,u;unsigned char basic[48];ULONG bytes=0;LONG status;
 if(!pid||!sample(o,1))return -1;for(i=0;i<o->n;i++)if(o->nodes[i].pid==pid){n=&o->nodes[i];if(!GetProcessTimes(n->process,&c,&e,&k,&u)||filetime(c)!=n->creation){fail(o,"ROLE_IDENTITY_CHANGED",0);return -1;}n->roles|=role;return (int)i;}
 if(o->n==CAP){fail(o,"NODE_CAP",0);return -1;}n=&o->nodes[o->n++];memset(n,0,sizeof(*n));n->pid=pid;n->roles=role;n->file_trust=TRUST_E_NOSIGNATURE;
 n->process=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,pid);if(!n->process){n->error=GetLastError();return (int)(o->n-1);}
 if(!GetProcessTimes(n->process,&c,&e,&k,&u)){n->error=GetLastError();return (int)(o->n-1);}n->creation=filetime(c);n->identity=1;
 status=o->query?o->query(n->process,0,basic,sizeof(basic),&bytes):((LONG)0xC0000002L);
 if(!status&&bytes==48){ULONGLONG parent;memcpy(&parent,basic+40,8);if(parent<=MAXDWORD)n->parent=(DWORD)parent;}else n->error=(DWORD)status;
 if(!ProcessIdToSessionId(pid,&n->session))n->error=GetLastError();n->host_status=o->query?o->query(n->process,49,&n->host_raw,sizeof(n->host_raw),&n->host_bytes):((LONG)0xC0000002L);
 token(o,n);if(!o->frozen)image(o,n);if(!o->frozen)metrics(o,n);return (int)(o->n-1);
}
static int host_candidate(const NODE *n,DWORD *pid){ULONGLONG value=n->host_raw&~3ULL;
 if(n->host_status||n->host_bytes!=8||(n->host_raw&3ULL)!=1||!value||value>MAXDWORD)return 0;*pid=(DWORD)value;return 1;}
static void print(OBS *o,FILE *f,DWORD seed){unsigned i,remaining=0;ULONGLONG sum=0;int complete=!o->frozen;unsigned host_candidates=0;
 fprintf(f,"{\"kind\":\"READ_ONLY_CONSOLE_DIAGNOSTIC_NOT_ADMISSION\",\"seed_pid\":%lu,\"observer_pid\":%lu,\"first\":\"%s\",\"first_error\":%lu,\"own_peak\":%lu,\"attached\":%s,\"attach_error\":%lu,\"before_detach_client_count\":%lu,\"freeconsole_returned\":%s,\"freeconsole_error\":%lu,\"after_detach_client_count\":%lu,\"window_process\":%lu,\"client_count\":%lu,\"clients\":[",seed,GetCurrentProcessId(),o->first,o->first_error,o->own_peak,o->attached?"true":"false",o->attach_error,o->before_clients,o->detach_ok?"true":"false",o->detach_error,o->after_detach_clients,o->window_pid,o->members);
 for(i=0;o->members<=16&&i<o->members;i++)fprintf(f,"%s%lu",i?",":"",o->ids[i]);fputs("],\"nodes\":[",f);
 for(i=0;i<o->n;i++){NODE *n=&o->nodes[i];if(n->roles&R_NATIVE_HOST)host_candidates++;if(n->roles&(R_SEED|R_NATIVE_HOST)){if(!n->identity||!n->stable||!n->metrics)complete=0;else sum+=n->count;}
 if(n->process)remaining++;fprintf(f,"%s{\"pid\":%lu,\"roles\":%lu,\"original_handle_identity\":%s,\"creation\":%llu,\"creation_after\":%llu,\"live_stable\":%s,\"parent_metadata\":%lu,\"session\":%lu,\"query_error\":%lu,\"class49_status\":%ld,\"class49_bytes\":%lu,\"class49_raw\":%llu,\"class49_after_queried\":%s,\"class49_after_status\":%ld,\"class49_after_raw\":%llu,\"token_known\":%s,\"token_error\":%lu,\"elevated\":%s,\"token_user\":",i?",":"",n->pid,n->roles,n->identity?"true":"false",n->creation,n->after_creation,n->stable?"true":"false",n->parent,n->session,n->error,n->host_status,n->host_bytes,n->host_raw,n->after_queried?"true":"false",n->host_after_status,n->host_after,n->token_known?"true":"false",n->token_error,n->elevated?"true":"false");json_wide(f,n->sid);fputs(",\"image_path\":",f);json_wide(f,n->image);
 fprintf(f,",\"file_identity_known\":%s,\"file_volume\":%lu,\"file_id_high\":%lu,\"file_id_low\":%lu,\"file_bytes_high\":%lu,\"file_bytes_low\":%lu,\"hash_known\":%s,\"sha256\":\"%s\",\"offline_file_trust_queried\":false,\"offline_file_trust\":null,\"mapped_image_provenance\":\"UNPROVEN\",\"metrics_known\":%s,\"metric_error\":%lu,\"handles\":%lu,\"private_bytes\":%Iu,\"working_set\":%Iu,\"sample_tick_ms\":%llu}",n->image_known?"true":"false",n->file.dwVolumeSerialNumber,n->file.nFileIndexHigh,n->file.nFileIndexLow,n->file.nFileSizeHigh,n->file.nFileSizeLow,n->hash_known?"true":"false",n->hash,n->metrics?"true":"false",n->metric_error,n->count,n->private_bytes,n->ws,n->tick);}
 fprintf(f,"],\"unique_seed_and_class49_candidate_sum_known\":%s,\"unique_seed_and_class49_candidate_handles\":%llu,\"retained\":%s,\"close_error\":%lu,\"controlled_original_refs_remaining\":%u,\"scope\":\"CURRENT_HELD_OBJECTS_SEQUENTIAL_SAMPLES; HOST_DISCOVERY_METADATA_NOT_AUTHENTICATION; TERMINAL_UI_COVERAGE_NOT_ASSERTED\",\"unsampled_peak\":\"UNKNOWN\",\"TPM\":\"NOT_EXECUTED\",\"Q4\":\"NOT_EXECUTED\",\"authority\":\"DENY\"}\n",complete&&host_candidates==1?"true":"false",sum,o->retained?"true":"false",o->close_error,remaining+(o->temporary?1U:0U)+(o->sha?1U:0U)+(o->hash?1U:0U));
}
static int observe(DWORD seed,int attach,FILE *out,ULONGLONG expected){OBS *o=&observation;unsigned i;int index;init(o);SetConsoleCtrlHandler(stop_handler,TRUE);
 index=bind_role(o,seed,R_SEED);if(index<0)goto done;if(expected&&(!o->nodes[index].identity||o->nodes[index].creation!=expected)){fail(o,"SEED_CREATION_MISMATCH",0);goto done;}if(o->nodes[index].parent&&!o->frozen)bind_role(o,o->nodes[index].parent,R_ANCESTOR);if(attach&&!o->frozen){o->before_clients=GetConsoleProcessList(o->ids,16);o->detach_ok=FreeConsole()!=0;o->detach_error=o->detach_ok?0:GetLastError();o->after_detach_clients=GetConsoleProcessList(o->ids,16);o->attached=AttachConsole(seed)!=0;o->attach_error=o->attached?0:GetLastError();}
 if(!o->frozen){bind_role(o,GetCurrentProcessId(),R_OBSERVER);if(GetConsoleWindow())GetWindowThreadProcessId(GetConsoleWindow(),&o->window_pid);
 o->members=GetConsoleProcessList(o->ids,16);if(o->window_pid)bind_role(o,o->window_pid,R_WINDOW);
 if(o->members>0&&o->members<=16)for(i=0;i<o->members&&!o->frozen;i++)bind_role(o,o->ids[i],R_CLIENT);
 /* Class49 is reverse-engineered metadata. Preserve raw flags; no setter or auth claim. */
 for(i=0;i<o->n&&!o->frozen;i++){NODE *n=&o->nodes[i];DWORD candidate;if(host_candidate(n,&candidate))bind_role(o,candidate,(n->roles&(R_SEED|R_CLIENT|R_WINDOW))?R_NATIVE_HOST:R_BASELINE_HOST);}
 for(i=0;i<o->n&&!o->frozen;i++){NODE *n=&o->nodes[i];if(!n->process)continue;n->after_queried=1;n->host_after_status=o->query?o->query(n->process,49,&n->host_after,8,&n->host_after_bytes):((LONG)0xC0000002L);metrics(o,n);if(n->host_status==0&&n->host_after_status==0&&n->host_raw!=n->host_after)fail(o,"CONSOLE_ASSOCIATION_CHANGED",0);}}
 done:if(!o->retained){close_ref(o,&o->temporary);for(i=0;i<o->n&&!o->retained;i++)close_ref(o,&o->nodes[i].process);if(o->sha&&!o->retained){NTSTATUS s=BCryptCloseAlgorithmProvider(o->sha,0);if(s!=0){o->retained=1;fail(o,"SHA_CLOSE_UNKNOWN",(DWORD)s);}else o->sha=NULL;}}
 print(o,out,seed);return o->retained?3:o->frozen?2:0;
}
int wmain(int argc,wchar_t **argv){DWORD seed;wchar_t *end;FILE *out=NULL;int rc;
 if(argc==4&&(!wcscmp(argv[1],L"--observe")||!wcscmp(argv[1],L"--observe-inherited"))){ULONGLONG expected,v=_wcstoui64(argv[2],&end,10);if(*end||!v||v>MAXDWORD)return 2;seed=(DWORD)v;expected=_wcstoui64(argv[3],&end,10);if(*end||!expected)return 2;rc=observe(seed,!wcscmp(argv[1],L"--observe"),stdout,expected);if(rc==3){puts("UNKNOWN_ORIGINAL_REFERENCES_RETAINED_NO_RETRY_NO_FORCE_CLOSE");fflush(stdout);for(;;)Sleep(1000);}return rc;}
 if(argc==3&&!wcscmp(argv[1],L"--fixture-output")){if(_wfopen_s(&out,argv[2],L"wx"))return 2;rc=observe(GetCurrentProcessId(),0,out,0);if(fclose(out))return 3;return rc;}
 puts("DENY: explicit diagnostic mode required; no TPM APIs");return 2;
}
