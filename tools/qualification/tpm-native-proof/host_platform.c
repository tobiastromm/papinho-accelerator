#include "host_platform.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <limits.h>
static const unsigned char operator_sid[]={1,5,0,0,0,0,0,5,21,0,0,0,222,210,212,46,186,166,219,85,236,129,65,100,84,4,0,0};
/* SID generated from the existing authorized profile; fixture checks exact string. */
void hp_init(TP_PLATFORM *p){unsigned i;memset(p,0,sizeof(*p));tp_init(&p->proof);p->proof.guard_data=p;p->proof.boundary=hp_boundary;for(i=0;i<TP_FILES;i++)p->files[i].handle=INVALID_HANDLE_VALUE;}
void hp_hex(const unsigned char *b,unsigned n,char *out){static const char hex[]="0123456789abcdef";unsigned i;for(i=0;i<n;i++){out[2*i]=hex[b[i]>>4];out[2*i+1]=hex[b[i]&15];}out[2*n]=0;}
int hp_hex_input(const wchar_t *s,char out[65]){unsigned i;if(wcslen(s)!=64)return 0;for(i=0;i<64;i++){if(!((s[i]>=L'0'&&s[i]<=L'9')||(s[i]>=L'a'&&s[i]<=L'f')))return 0;out[i]=(char)s[i];}out[64]=0;return 1;}
static int host_fail(TP_PLATFORM *p,const char *where){return tp_fail(&p->proof,where,GetLastError());}
static int close_handle(TP_PLATFORM *p,HANDLE *h){if(!*h||*h==INVALID_HANDLE_VALUE)return 1;if(!CloseHandle(*h)){p->proof.retained=1;return host_fail(p,"WINDOWS_CLOSE_UNKNOWN");}*h=NULL;p->proof.controlled_refs--;return 1;}
static int close_event(TP_PLATFORM *p,EVT_HANDLE *h){if(!*h)return 1;if(!EvtClose(*h)){p->proof.retained=1;return host_fail(p,"EVENTLOG_CLOSE_UNKNOWN");}*h=NULL;p->proof.controlled_refs--;return 1;}
static ULONGLONG ft(const FILETIME *f){return ((ULONGLONG)f->dwHighDateTime<<32)|f->dwLowDateTime;}
int hp_token(TP_PLATFORM *p){HANDLE thread=NULL;TOKEN_STATISTICS a,b;DWORD used=0;unsigned char ids[24],admin_sid[SECURITY_MAX_SID_SIZE];DWORD sid_bytes=sizeof(admin_sid);BOOL enabled=FALSE;TOKEN_USER *user=(TOKEN_USER *)p->token_buffer;int ok=0;TP_OWNER *o=&p->proof;
 if(o->frozen)return 0;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)){p->token=thread;o->controlled_refs++;tp_fail(o,"IMPERSONATION_DENY",0);goto done;}
 if(thread){p->token=thread;o->controlled_refs++;o->retained=1;return tp_fail(o,"THREAD_TOKEN_CONTRADICTORY",GetLastError());}
 if(GetLastError()!=ERROR_NO_TOKEN)return host_fail(p,"THREAD_TOKEN_QUERY");
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&p->token)){if(p->token){o->controlled_refs++;o->retained=1;}return host_fail(p,"PROCESS_TOKEN_QUERY");}o->controlled_refs++;
 if(!tp_sample(o,"TOKEN_HELD"))goto done;
 if(!GetTokenInformation(p->token,TokenStatistics,&a,sizeof(a),&used)){host_fail(p,"TOKEN_STATISTICS_QUERY");goto done;}if(used!=sizeof(a)){tp_fail(o,"TOKEN_STATISTICS_SIZE",0);goto done;}
 if(!GetTokenInformation(p->token,TokenUser,p->token_buffer,sizeof(p->token_buffer),&used)||used<sizeof(TOKEN_USER)||used>sizeof(p->token_buffer)){host_fail(p,"TOKEN_USER_QUERY");goto done;}
 if((unsigned char *)user->User.Sid<p->token_buffer||(unsigned char *)user->User.Sid>p->token_buffer+used-8||!IsValidSid(user->User.Sid)||GetLengthSid(user->User.Sid)>used-(DWORD)((unsigned char *)user->User.Sid-p->token_buffer)||!EqualSid(user->User.Sid,(PSID)operator_sid)){tp_fail(o,"AUTHORIZED_OPERATOR_DENY",0);goto done;}
 if(!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin_sid,&sid_bytes)||!CheckTokenMembership(NULL,admin_sid,&enabled)){host_fail(p,"ADMIN_TOKEN_QUERY");goto done;}if(!enabled){tp_fail(o,"ELEVATED_ADMIN_REQUIRED",0);goto done;}
 if(!GetTokenInformation(p->token,TokenStatistics,&b,sizeof(b),&used)){host_fail(p,"TOKEN_STATISTICS_QUERY");goto done;}if(used!=sizeof(b)){tp_fail(o,"TOKEN_STATISTICS_SIZE",0);goto done;}
 if(memcmp(&a.TokenId,&b.TokenId,sizeof(LUID))||memcmp(&a.AuthenticationId,&b.AuthenticationId,sizeof(LUID))||memcmp(&a.ModifiedId,&b.ModifiedId,sizeof(LUID))){tp_fail(o,"TOKEN_CHANGED_DURING_QUERY",0);goto done;}
 memcpy(ids,&b.TokenId,8);memcpy(ids+8,&b.AuthenticationId,8);memcpy(ids+16,&b.ModifiedId,8);
 if(p->token_bound&&!tp_eq(ids,p->token_ids,24)){tp_fail(o,"TOKEN_CONTINUITY_DENY",0);goto done;}if(!p->token_bound){memcpy(p->token_ids,ids,24);p->token_bound=1;}ok=1;
done:if(!close_handle(p,&p->token))ok=0;return ok;
}
static int digits(const wchar_t *p,unsigned n,unsigned *out){unsigned i,x=0;for(i=0;i<n;i++){if(p[i]<L'0'||p[i]>L'9')return 0;x=x*10+(unsigned)(p[i]-L'0');}*out=x;return 1;}
int hp_utc_filetime(const wchar_t *s,ULONGLONG *out){SYSTEMTIME t;FILETIME f;unsigned y,m,d,h,mi,se,fraction=0,n=0,oh=0,om=0;size_t i,len=wcslen(s);ULONGLONG value,offset;int sign=0;
 if(len<20||len>40||s[4]!=L'-'||s[7]!=L'-'||s[10]!=L'T'||s[13]!=L':'||s[16]!=L':'||!digits(s,4,&y)||!digits(s+5,2,&m)||!digits(s+8,2,&d)||!digits(s+11,2,&h)||!digits(s+14,2,&mi)||!digits(s+17,2,&se))return 0;
 memset(&t,0,sizeof(t));t.wYear=(WORD)y;t.wMonth=(WORD)m;t.wDay=(WORD)d;t.wHour=(WORD)h;t.wMinute=(WORD)mi;t.wSecond=(WORD)se;i=19;
 if(s[i]==L'.'){i++;while(i<len&&s[i]>=L'0'&&s[i]<=L'9'){if(n==7)return 0;fraction=fraction*10+(unsigned)(s[i++]-L'0');n++;}if(!n)return 0;}while(n++<7)fraction*=10;
 if(s[i]==L'Z'){if(i+1!=len)return 0;}else{if(s[i]!=L'+'&&s[i]!=L'-')return 0;sign=s[i]==L'+'?1:-1;if(i+6!=len||s[i+3]!=L':'||!digits(s+i+1,2,&oh)||!digits(s+i+4,2,&om)||oh>23||om>59)return 0;}
 if(!SystemTimeToFileTime(&t,&f))return 0;value=ft(&f);if(value>ULLONG_MAX-fraction)return 0;value+=fraction;offset=((ULONGLONG)oh*60+om)*600000000ULL;
 if(sign==1){if(value<offset)return 0;value-=offset;}else if(sign==-1){if(value>ULLONG_MAX-offset)return 0;value+=offset;}*out=value;return 1;
}
static int wide_hash(TP_PLATFORM *p,const wchar_t *s,const char *expected){char utf8[2048],hex[65];unsigned char hash[32];int n=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,s,-1,utf8,sizeof(utf8),NULL,NULL);if(n<1)return host_fail(p,"METADATA_UTF8");if(!tp_hash(&p->proof,(unsigned char *)utf8,(size_t)n-1,hash))return 0;hp_hex(hash,32,hex);return !strcmp(hex,expected)?1:tp_fail(&p->proof,"HOST_METADATA_DENY",0);}
static int event_string_bounded(const void *buffer,DWORD bytes,const wchar_t *s){ULONG_PTR base=(ULONG_PTR)buffer,value=(ULONG_PTR)s;size_t n,i;if(!s||value<base+3*sizeof(EVT_VARIANT)||value>=base+bytes||(value-base)%sizeof(wchar_t))return 0;n=(base+bytes-value)/sizeof(wchar_t);for(i=0;i<n;i++)if(!s[i])return 1;return 0;}
int hp_boot(TP_PLATFORM *p){wchar_t computer[256];DWORD n=256,got=0,bytes=0,properties=0;const wchar_t *paths[]={L"Event/System/EventRecordID",L"Event/System/Computer",L"Event/EventData/Data[@Name='StartTime']"};union {ULONGLONG align[1024];EVT_VARIANT values[128];} buffer;EVT_VARIANT *v=buffer.values;FILETIME c,e,k,u;ULONGLONG observed=0,expected=0;TP_OWNER *o=&p->proof;int ok=0;
 if(o->frozen)return 0;if(!GetComputerNameW(computer,&n))return host_fail(p,"HOST_NAME_QUERY");if(!wide_hash(p,computer,TP_HOST_SHA))return 0;
 p->query=EvtQuery(NULL,L"System",L"*[System[Provider[@Name='Microsoft-Windows-Kernel-General'] and EventID=12]]",EvtQueryChannelPath|EvtQueryReverseDirection);if(!p->query)return host_fail(p,"BOOT_EVENT_QUERY");o->controlled_refs++;
 {BOOL next=EvtNext(p->query,1,&p->event,0,0,&got);DWORD error=next?0:GetLastError();if(p->event)o->controlled_refs++;if(!next||got!=1||!p->event){if(p->event)o->retained=1;tp_fail(o,"BOOT_EVENT_READ",error);goto done;}}
 p->render=EvtCreateRenderContext(3,paths,EvtRenderContextValues);if(!p->render){host_fail(p,"BOOT_RENDER_CONTEXT");goto done;}o->controlled_refs++;
 memset(&buffer,0,sizeof(buffer));if(!EvtRender(p->render,p->event,EvtRenderEventValues,sizeof(buffer),&buffer,&bytes,&properties)||properties!=3||bytes>sizeof(buffer)||bytes<3*sizeof(EVT_VARIANT)){host_fail(p,"BOOT_EVENT_VALUES");goto done;}
 if(v[0].Type!=EvtVarTypeUInt64||v[0].UInt64Val!=TP_BOOT_RECORD||v[1].Type!=EvtVarTypeString||!event_string_bounded(&buffer,bytes,v[1].StringVal)){tp_fail(o,"BOOT_RECORD_DENY",0);goto done;}
 if(!wide_hash(p,v[1].StringVal,TP_EVENT_HOST_SHA))goto done;
 if(v[2].Type==EvtVarTypeFileTime)observed=v[2].FileTimeVal;
 else if(v[2].Type!=EvtVarTypeString||!event_string_bounded(&buffer,bytes,v[2].StringVal)||!hp_utc_filetime(v[2].StringVal,&observed)){tp_fail(o,"BOOT_START_DENY",0);goto done;}
 if(!hp_utc_filetime(L"2026-10-26T21:35:16.5000000Z",&expected)||expected!=observed){tp_fail(o,"BOOT_START_DENY",0);goto done;}
 if(!GetProcessTimes(GetCurrentProcess(),&c,&e,&k,&u)){host_fail(p,"SELF_CREATION_QUERY");goto done;}
 p->pid=GetCurrentProcessId();p->creation=ft(&c);if(!p->creation){tp_fail(o,"SELF_CREATION_UNKNOWN",0);goto done;}
 _snprintf_s(p->live_binding,sizeof(p->live_binding),_TRUNCATE,"%llu|%lu|%llu",TP_BOOT_RECORD,p->pid,p->creation);ok=tp_sample(o,"METADATA_HELD");
done:if(o->retained)return 0;if(!close_event(p,&p->render)||!close_event(p,&p->event)||!close_event(p,&p->query))return 0;return ok;
}
#include "host_admin.inc"

static int file_current(TP_PLATFORM *p,TP_FILE *f){BY_HANDLE_FILE_INFORMATION id;LARGE_INTEGER size;if(!GetFileInformationByHandle(f->handle,&id)||!GetFileSizeEx(f->handle,&size))return host_fail(p,"PINNED_FILE_ID_QUERY");
 if((id.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||id.dwVolumeSerialNumber!=f->id.dwVolumeSerialNumber||id.nFileIndexHigh!=f->id.nFileIndexHigh||id.nFileIndexLow!=f->id.nFileIndexLow||size.QuadPart!=f->bytes)return tp_fail(&p->proof,"PINNED_FILE_CHANGED",0);return 1;
}
int hp_context_current(TP_PLATFORM *p,const char *phase){unsigned char material[512],digest[32];int n;
 if(!p->context_bound&&strcmp(phase,"CHALLENGE"))return 1;
 n=_snprintf_s((char *)material,sizeof(material),_TRUNCATE,"%s|%s|%s",p->request_hash,p->approval_hash,p->live_binding);
 if(n<0||(unsigned)n+58>sizeof(material))return tp_fail(&p->proof,"CONTEXT_BINDING_SIZE",0);
 memcpy(material+n,p->token_ids,24);memcpy(material+n+24,p->proof.public_name,34);
 if(!tp_hash(&p->proof,material,(size_t)n+58,digest))return 0;
 if(!p->context_bound){memcpy(p->expected_context,digest,32);memcpy(p->proof.context_binding,digest,32);p->context_bound=1;return 1;}
 return tp_eq(digest,p->expected_context,32)&&tp_eq(digest,p->proof.context_binding,32)?1:tp_fail(&p->proof,"LIVE_CONTEXT_CHANGED",0);
}
int hp_console_stop(TP_PLATFORM *p){TP_OWNER *o=&p->proof;INPUT_RECORD input[32];DWORD count=0,i,pending=0;HANDLE in=GetStdHandle(STD_INPUT_HANDLE);
 if(o->frozen)return 0;if(InterlockedCompareExchange(&o->stop,0,0))return tp_fail(o,"OWNER_STOP",0);
 if(!PeekConsoleInputW(in,input,32,&count)||!GetNumberOfConsoleInputEvents(in,&pending))return host_fail(p,"CONSOLE_INPUT_QUERY");
 if(pending>32)return tp_fail(o,"CONSOLE_INPUT_BACKLOG_DENY",0);
 for(i=0;i<count;i++)if(input[i].EventType==KEY_EVENT&&input[i].Event.KeyEvent.bKeyDown&&input[i].Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE){InterlockedExchange(&o->stop,1);return tp_fail(o,"OWNER_STOP",0);}return 1;
}
int hp_files_current(TP_PLATFORM *p){unsigned i;if(p->proof.frozen)return 0;for(i=0;i<TP_FILES;i++)if(p->files[i].handle!=INVALID_HANDLE_VALUE&&p->files[i].handle&&!file_current(p,&p->files[i]))return 0;return 1;}
int hp_boundary(TP_OWNER *o,const char *phase){TP_PLATFORM *p=o->guard_data;return hp_console_stop(p)&&hp_token(p)&&hp_admin(p)&&hp_files_current(p)&&hp_context_current(p,phase);}
static int file_open(TP_PLATFORM *p,unsigned index,const wchar_t *path,unsigned max,char hex[65],unsigned char *data,unsigned cap,unsigned *length){TP_FILE *f=&p->files[index];LARGE_INTEGER size;unsigned char block[4096],hash[32];DWORD got=0,offset=0;NTSTATUS status;TP_OWNER *o=&p->proof;int ok=0;
 if(o->frozen||f->handle!=INVALID_HANDLE_VALUE||o->work_hash)return 0;
 f->handle=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(f->handle==INVALID_HANDLE_VALUE)return host_fail(p,"ARTIFACT_OPEN");o->controlled_refs++;
 if(GetFileType(f->handle)!=FILE_TYPE_DISK)return tp_fail(o,"ARTIFACT_NOT_DISK",0);if(!GetFileInformationByHandle(f->handle,&f->id)||!GetFileSizeEx(f->handle,&size))return host_fail(p,"ARTIFACT_INFO");
 if((f->id.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||size.QuadPart<=0||size.QuadPart>max||(data&&(ULONGLONG)size.QuadPart>=cap))return tp_fail(o,"ARTIFACT_SIZE_REPARSE_DENY",0);f->bytes=(DWORD)size.QuadPart;f->info_known=1;
 status=BCryptCreateHash(o->sha,&o->work_hash,o->hash_object,o->sha_object,NULL,0,0);if(o->work_hash)o->controlled_refs++;if(status!=0||!o->work_hash){if(o->work_hash)o->retained=1;return tp_fail(o,"FILE_HASH_CREATE",(unsigned long)status);}
 if(!tp_sample(o,"ARTIFACT_HASH_HELD"))goto done;
 while(offset<f->bytes){DWORD want=f->bytes-offset;if(want>sizeof(block))want=sizeof(block);if(!ReadFile(f->handle,block,want,&got,NULL)){host_fail(p,"ARTIFACT_READ_FAILED");goto done;}if(got!=want){tp_fail(o,"ARTIFACT_SHORT_READ",0);goto done;}
 status=BCryptHashData(o->work_hash,block,got,0);if(status!=0){tp_fail(o,"FILE_HASH_DATA",(unsigned long)status);goto done;}if(data)memcpy(data+offset,block,got);offset+=got;}
 status=BCryptFinishHash(o->work_hash,hash,32,0);if(status!=0){tp_fail(o,"FILE_HASH_FINISH",(unsigned long)status);goto done;}hp_hex(hash,32,hex);strcpy_s(f->sha256,sizeof(f->sha256),hex);f->hash_known=1;if(data){data[offset]=0;*length=offset;}ok=file_current(p,f);
done:status=BCryptDestroyHash(o->work_hash);if(status!=0){o->retained=1;return tp_fail(o,"FILE_HASH_CLOSE_UNKNOWN",(unsigned long)status);}o->work_hash=NULL;o->controlled_refs--;SecureZeroMemory(o->hash_object,sizeof(o->hash_object));return ok;
}
static int path(TP_PLATFORM *p,const wchar_t *name,wchar_t out[MAX_PATH]){return swprintf_s(out,MAX_PATH,L"%s%s",p->folder,name)>0?1:tp_fail(&p->proof,"ARTIFACT_PATH",0);}
int hp_files(TP_PLATFORM *p,const char exe_sha[65],const char approval_sha[65]){wchar_t f[MAX_PATH];unsigned n=0;char hex[65];DWORD length=GetModuleFileNameW(NULL,p->executable,MAX_PATH);wchar_t *slash;
 if(!length||length>=MAX_PATH)return host_fail(p,"SELF_IMAGE_PATH");wcscpy_s(p->folder,MAX_PATH,p->executable);slash=wcsrchr(p->folder,L'\\');if(!slash)return tp_fail(&p->proof,"SELF_IMAGE_PATH",0);slash[1]=0;
 if(!file_open(p,0,p->executable,4194304,p->exe_hash,NULL,0,&n)||strcmp(p->exe_hash,exe_sha))return p->proof.frozen?0:tp_fail(&p->proof,"EXE_PIN",0);
 if(!path(p,L"OWNER-APPROVED-NATIVE-PROOF.json",f)||!file_open(p,1,f,TJ_BYTES-1,p->approval_hash,(unsigned char *)p->approval.bytes,TJ_BYTES,&n))return 0;
 if(strcmp(p->approval_hash,approval_sha))return tp_fail(&p->proof,"APPROVAL_PIN",0);if(!tj_parse(&p->approval,n))return tp_fail(&p->proof,"APPROVAL_JSON",0);
 if(!path(p,L"NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json",f)||!file_open(p,2,f,TJ_BYTES-1,p->request_hash,(unsigned char *)p->request.bytes,TJ_BYTES,&n))return 0;
 if(!tj_parse(&p->request,n))return tp_fail(&p->proof,"REQUEST_JSON",0);
 if(!path(p,L"ek-certificate.PUBLIC.der",f)||!file_open(p,3,f,TP_CERT_MAX-1,hex,p->cert_bytes,sizeof(p->cert_bytes),&n))return 0;
 if(strcmp(hex,TP_CERT_SHA))return tp_fail(&p->proof,"CERT_PIN",0);return hp_request_contract(p)&&hp_approval_contract(p);
}
int hp_request_contract(TP_PLATFORM *p){TJ_DOC *d=&p->request;int meta=tj_get(d,0,"current_metadata_pins"),limits=tj_get(d,0,"administrative_resource_gate");unsigned long long n;
 if(!tj_is(d,tj_get(d,0,"console_binding_revision"),"C0-C9")||!tj_is(d,tj_get(d,0,"console_binding_proposal_sha256"),"7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985"))return tp_fail(&p->proof,"REQUEST_CONSOLE_POLICY",0);
 if(!tj_is(d,tj_get(d,0,"kind"),"OWNER_MANUAL_DIAGNOSTIC_REQUEST_NOT_APPROVAL")||!tj_is(d,tj_get(d,0,"executable_sha256"),p->exe_hash)||!tj_is(d,tj_get(d,meta,"operator_sid"),TP_OPERATOR)||!tj_is(d,tj_get(d,meta,"current_machine_name_sha256"),TP_HOST_SHA)||!tj_is(d,tj_get(d,meta,"boot_event_computer_sha256"),TP_EVENT_HOST_SHA)||!tj_is(d,tj_get(d,meta,"boot_start_utc"),TP_BOOT_START))return tp_fail(&p->proof,"REQUEST_IDENTITY_SCOPE",0);
 if(!tj_u64(d,tj_get(d,meta,"boot_record"),&n)||n!=TP_BOOT_RECORD||!tj_u64(d,tj_get(d,0,"direct_TBS_submit_maximum"),&n)||n!=12)return tp_fail(&p->proof,"REQUEST_LIMIT_SCOPE",0);
 if(!tj_u64(d,tj_get(d,limits,"native_own_sample_limit"),&n)||n!=128||!tj_u64(d,tj_get(d,limits,"native_before_observer_reserve_limit"),&n)||n!=124||!tj_u64(d,tj_get(d,limits,"shared_parent_console_sample_limit"),&n)||n!=1024)return tp_fail(&p->proof,"REQUEST_RESOURCE_SCOPE",0);return 1;
}
int hp_approval_contract(TP_PLATFORM *p){TJ_DOC *a=&p->approval;
 if(!tj_is(a,tj_get(a,0,"kind"),"OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY")||!tj_is(a,tj_get(a,0,"request_sha256"),p->request_hash)||!tj_is(a,tj_get(a,0,"executable_sha256"),p->exe_hash)||!tj_true(a,tj_get(a,0,"one_manual_execution"))||!tj_true(a,tj_get(a,0,"ack_unknown_revocation"))||!tj_true(a,tj_get(a,0,"ack_no_hardware_admission"))||!tj_true(a,tj_get(a,0,"ack_internal_tpm_accounting_unknown")))return tp_fail(&p->proof,"EXACT_APPROVAL_SCOPE",0);return 1;
}
int hp_confirm(TP_PLATFORM *p){wchar_t entered[160],expected[160];DWORD mode=0,n=0;HANDLE input=GetStdHandle(STD_INPUT_HANDLE);
 if(!GetConsoleMode(input,&mode))return host_fail(p,"INTERACTIVE_CONSOLE_REQUIRED");if(swprintf_s(expected,160,L"PROVE_EK_SESSION %S %lu %llu %lu %llu",p->request_hash,p->parent_pid,p->parent_creation,p->console_pid,p->console_creation)<0)return tp_fail(&p->proof,"CONFIRMATION_SHAPE",0);
 if(!p->parent_pid||!p->console_pid||!p->parent_creation||!p->console_creation)return tp_fail(&p->proof,"CONFIRMATION_ROLE_BINDING",0);
 printf("C0_BIND parent=%lu creation=%llu auxiliary=%lu creation=%llu window_metadata=%lu; CONSOLE_AUTH_AND_MAPPED_IMAGE_UNPROVEN; NO_ADMISSION\n",p->parent_pid,p->parent_creation,p->console_pid,p->console_creation,p->window_owner);
 printf("TRANSIENT SALTED EK MATHEMATICAL PROOF ONLY; revocation UNKNOWN; no NV/AK/hierarchy/Q4. One attempt; no retry. Type exactly: %ls\n",expected);fflush(stdout);
 if(!ReadConsoleW(input,entered,159,&n,NULL)||!n||n>=159)return host_fail(p,"CONFIRMATION_READ");entered[n]=0;while(n&&(entered[n-1]==L'\n'||entered[n-1]==L'\r'))entered[--n]=0;
 return !wcscmp(entered,expected)?1:tp_fail(&p->proof,"CONFIRMATION_DENY",0);
}
int hp_receipt(TP_PLATFORM *p){wchar_t name[MAX_PATH];char b[256];int length;DWORD wrote=0;TP_OWNER *o=&p->proof;
 if(!hp_console_stop(p))return 0;
 if(!path(p,L"NATIVE-PROOF-ATTEMPT.ONCE.receipt",name))return 0;if(p->token)return tp_fail(o,"OWNER_SLOT_BUSY",0);
 p->token=CreateFileW(name,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH,NULL);if(p->token==INVALID_HANDLE_VALUE){p->token=NULL;return host_fail(p,"ADMIN_RECEIPT_CREATE_NEW");}o->controlled_refs++;p->receipt_created=1;
 length=_snprintf_s(b,sizeof(b),_TRUNCATE,"REQUEST=%s\nAPPROVAL=%s\nNO_RETRY; NOT_Q4_AUTHORITY\n",p->request_hash,p->approval_hash);
 if(length<0){p->receipt_uncertain=1;tp_fail(o,"ADMIN_RECEIPT_ENCODING",0);}else{
 p->receipt_expected=(DWORD)length;
 if(!WriteFile(p->token,b,(DWORD)length,&wrote,NULL)){p->receipt_uncertain=1;host_fail(p,"ADMIN_RECEIPT_WRITE");}
 else if(wrote!=(DWORD)length){p->receipt_uncertain=1;tp_fail(o,"ADMIN_RECEIPT_SHORT_WRITE",0);}
 else if(!FlushFileBuffers(p->token)){p->receipt_uncertain=1;host_fail(p,"ADMIN_RECEIPT_FLUSH");}
 p->receipt_written=wrote;
 }
 if(!close_handle(p,&p->token))return 0;return !o->frozen;
}
int hp_load_certificate(TP_PLATFORM *p){unsigned char blob[512];DWORD n=0;BCRYPT_RSAKEY_BLOB *head=(BCRYPT_RSAKEY_BLOB *)blob;BCRYPT_KEY_HANDLE imported=NULL;NTSTATUS status;TP_OWNER *o=&p->proof;
 p->cert=CertCreateCertificateContext(X509_ASN_ENCODING,p->cert_bytes,p->files[3].bytes);if(!p->cert)return host_fail(p,"CERT_PARSE");o->controlled_refs++;
 if(!CryptImportPublicKeyInfoEx2(X509_ASN_ENCODING,&p->cert->pCertInfo->SubjectPublicKeyInfo,0,NULL,&imported)){if(imported){o->work_key=imported;o->controlled_refs++;o->retained=1;}return host_fail(p,"CERT_PUBLIC_KEY_IMPORT");}if(!imported)return tp_fail(o,"CERT_PUBLIC_KEY_IMPORT_NULL",0);o->work_key=imported;o->controlled_refs++;
 status=BCryptExportKey(imported,NULL,BCRYPT_RSAPUBLIC_BLOB,blob,sizeof(blob),&n,0);if(status!=0||n!=sizeof(*head)+259||head->Magic!=BCRYPT_RSAPUBLIC_MAGIC||head->BitLength!=2048||head->cbPublicExp!=3||head->cbModulus!=256||blob[sizeof(*head)]!=1||blob[sizeof(*head)+1]||blob[sizeof(*head)+2]!=1)return tp_fail(o,"EK_PUBLIC_PROFILE",(unsigned long)status);
 if(!tp_rsa_import(o,blob+sizeof(*head)+3))return 0;
 status=BCryptDestroyKey(imported);if(status!=0){o->retained=1;return tp_fail(o,"CERT_IMPORTED_CLOSE_UNKNOWN",(unsigned long)status);}o->work_key=NULL;o->controlled_refs--;
 if(!CertFreeCertificateContext(p->cert)){o->retained=1;return host_fail(p,"CERT_CLOSE_UNKNOWN");}p->cert=NULL;o->controlled_refs--;return tp_sample(o,"CERT_EK_MATCH_READY");
}
int hp_close(TP_PLATFORM *p){unsigned i;if(p->proof.retained)return 0;p->proof.stage="PLATFORM_CLOSE";
 if(!close_handle(p,&p->role_image)||!close_handle(p,&p->console)||!close_handle(p,&p->parent)||!close_event(p,&p->render)||!close_event(p,&p->event)||!close_event(p,&p->query)||!close_handle(p,&p->token))return 0;
 if(p->cert){if(!CertFreeCertificateContext(p->cert)){p->proof.retained=1;return host_fail(p,"CERT_CLOSE_UNKNOWN");}p->cert=NULL;p->proof.controlled_refs--;}
 for(i=0;i<TP_FILES;i++)if(!close_handle(p,&p->files[i].handle))return 0;return 1;
}
void hp_print(const TP_PLATFORM *p,const char *transport_kind){const TP_OWNER *o=&p->proof;unsigned i;char name[71],hex[69];if(o->public_name[0]==0&&o->public_name[1]==11){hp_hex(o->public_name,34,hex);_snprintf_s(name,sizeof(name),_TRUNCATE,"\"%s\"",hex);}else strcpy_s(name,sizeof(name),"null");
 printf("{\"first\":\"%s\",\"first_stage\":\"%s\",\"first_native\":%lu,\"observer_query_attempted\":%s,\"observer_query_succeeded\":%s,\"observer_own_count\":",o->first,o->first_stage,o->first_native,p->observer_attempted?"true":"false",p->observer_known?"true":"false");
 if(p->observer_known)printf("%lu",p->observer_count);else printf("null");
 printf(",\"observer_query_error\":%lu,\"observer_reserve_limit\":124,\"proof\":%s,\"request_sha256\":\"%s\",\"approval_sha256\":\"%s\",\"live_binding\":\"%s\",\"calls\":%u,\"transport\":\"%s\",\"public_area_name\":%s,\"session_end\":\"%s\",\"context_opened\":%s,\"context_close\":%s,\"context_state\":\"%s\",\"retained\":%s,\"receipt_created\":%s,\"receipt_uncertain\":%s,\"controlled_refs\":%u,\"sampled_peak\":%u,\"unsampled_peak\":\"UNKNOWN\",\"revocation\":\"UNKNOWN\",\"device_generation\":\"NOT_ESTABLISHED\",\"NV_mutations\":\"NONE\",\"Q4\":\"NOT_EXECUTED\",\"authority\":\"DENY\",\"no_retry\":true,\"samples\":[",p->observer_error,o->proof?"true":"false",p->request_hash,p->approval_hash,p->live_binding,o->calls,transport_kind,name,o->session_ended?"AUTHENTICATED_CONTINUE_CLEAR_SEMANTIC":"NOT_CONFIRMED",o->context_opened?"true":"false",o->context_closed?"true":"false",o->context_closed?"CLOSED_CONFIRMED":o->context?"UNKNOWN_RETAINED":o->context_attempted?"OPEN_FAILED_NO_HANDLE":"NOT_OPENED",o->retained?"true":"false",p->receipt_created?"true":"false",p->receipt_uncertain?"true":"false",o->controlled_refs,o->peak);
 for(i=0;i<o->sample_count;i++){const TP_SAMPLE *s=&o->samples[i];printf("%s{\"phase\":\"%s\",\"tick_ms\":%llu,\"known\":%s,\"count\":",i?",":"",s->phase,s->tick,s->known?"true":"false");
 if(s->known)printf("%lu",s->count);else printf("null");printf(",\"error\":%lu,\"controlled_refs\":%u,\"memory_known\":%s,\"private_bytes\":",s->error,s->own_refs,s->memory_known?"true":"false");
 if(s->memory_known)printf("%Iu",s->private_bytes);else printf("null");printf(",\"working_set\":");if(s->memory_known)printf("%Iu",s->working_set);else printf("null");printf(",\"memory_error\":%lu}",s->memory_error);}
 printf("],\"observer_close_error\":%lu,\"observer_refs_retained\":%s,\"receipt_written\":%lu,\"receipt_expected\":%lu,\"topology_parent\":%lu,\"topology_console\":%lu,\"topology_count\":%lu,\"topology_clients\":[",p->observer_close_error,o->retained&&(p->parent||p->console)?"true":"false",p->receipt_written,p->receipt_expected,p->topology_parent,p->topology_console,p->topology_count);for(i=0;p->topology_count<=8&&i<p->topology_count;i++)printf("%s%lu",i?",":"",p->topology_ids[i]);printf("],\"console_binding_revision\":\"C0-C9\",\"window_owner_metadata\":%lu,\"role_file_verification_mask_at_last_admin_attempt\":%u,\"native_console_raw\":%llu,\"parent_console_raw\":%llu,\"administrative_samples\":[",p->window_owner,p->role_file_verified_mask,p->self_console_raw,p->console_raw);
 for(i=0;i<p->admin_count;i++){const TP_ADMIN_SAMPLE *s=&p->admins[i];printf("%s{\"parent\":%lu,\"parent_creation\":%llu,\"handles\":%lu,\"console\":%lu,\"console_creation\":%llu,\"console_handles\":%lu,\"tick_ms\":%llu}",i?",":"",s->parent_pid,s->parent_creation,s->parent_handles,s->console_pid,s->console_creation,s->console_handles,s->tick);}printf("] ,\"artifact_observations\":[");
 for(i=0;i<TP_FILES;i++){const TP_FILE *f=&p->files[i];printf("%s{\"role_index\":%u,\"native_identity_known\":%s,\"volume\":",i?",":"",i,f->info_known?"true":"false");
 if(f->info_known)printf("%lu,\"file_id_high\":%lu,\"file_id_low\":%lu,\"bytes\":%lu",f->id.dwVolumeSerialNumber,f->id.nFileIndexHigh,f->id.nFileIndexLow,f->bytes);else printf("null,\"file_id_high\":null,\"file_id_low\":null,\"bytes\":null");
 printf(",\"content_hash_known\":%s,\"sha256\":",f->hash_known?"true":"false");if(f->hash_known)printf("\"%s\"",f->sha256);else printf("null");printf("}");}
 printf("]}\n");fflush(stdout);
}
