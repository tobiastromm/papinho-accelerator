#include "host_platform.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static unsigned checks;
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u %s\n",(unsigned)__LINE__,#x);return 0;}}while(0)
static struct {int write_mode,close_mode;TP_PLATFORM *owner;volatile LONG creates,writes,peeks;} fault;
static BOOL f_peek(HANDLE h,PINPUT_RECORD r,DWORD n,LPDWORD read){(void)h;(void)r;(void)n;InterlockedIncrement(&fault.peeks);*read=0;return TRUE;}
static BOOL f_events(HANDLE h,LPDWORD n){(void)h;*n=0;return TRUE;}
static BOOL f_write(HANDLE h,LPCVOID b,DWORD n,LPDWORD used,LPOVERLAPPED o){InterlockedIncrement(&fault.writes);if(fault.write_mode==1){*used=0;SetLastError(ERROR_DISK_FULL);return FALSE;}if(fault.write_mode==2){*used=0;SetLastError(999);return TRUE;}return WriteFile(h,b,n,used,o);}
static BOOL f_close(HANDLE h){if(fault.close_mode&&fault.owner&&h==fault.owner->token){SetLastError(6);return FALSE;}return CloseHandle(h);}
static HANDLE f_create(LPCWSTR name,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sa,DWORD mode,DWORD flags,HANDLE templateFile){InterlockedIncrement(&fault.creates);return CreateFileW(name,access,share,sa,mode,flags,templateFile);}
#define PeekConsoleInputW f_peek
#define GetNumberOfConsoleInputEvents f_events
#define WriteFile f_write
#define CloseHandle f_close
#define CreateFileW f_create
#include "host_platform.c"
static TP_PLATFORM p,retained;
static char canonical[TJ_BYTES];
static int request_model(void){int n;memset(p.exe_hash,'a',64);p.exe_hash[64]=0;memset(p.request_hash,'c',64);p.request_hash[64]=0;
 n=_snprintf_s(p.request.bytes,TJ_BYTES,_TRUNCATE,"{\"kind\":\"OWNER_MANUAL_DIAGNOSTIC_REQUEST_NOT_APPROVAL\",\"executable_sha256\":\"%s\",\"direct_TBS_submit_maximum\":12,\"console_binding_revision\":\"C0-C9\",\"console_binding_proposal_sha256\":\"7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985\",\"current_metadata_pins\":{\"operator_sid\":\"%s\",\"current_machine_name_sha256\":\"%s\",\"boot_event_computer_sha256\":\"%s\",\"boot_record\":237900,\"boot_start_utc\":\"%s\"},\"administrative_resource_gate\":{\"native_own_sample_limit\":128,\"native_before_observer_reserve_limit\":124,\"shared_parent_console_sample_limit\":1024},\"UNIT_TEST_MODEL_ONLY\":true}",p.exe_hash,TP_OPERATOR,TP_HOST_SHA,TP_EVENT_HOST_SHA,TP_BOOT_START);
 C(n>0&&tj_parse(&p.request,(size_t)n));return 1;
}
static int approval_model(const char *literal){int n=_snprintf_s(p.approval.bytes,TJ_BYTES,_TRUNCATE,"{\"kind\":\"OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY\",\"request_sha256\":\"%s\",\"executable_sha256\":\"%s\",\"one_manual_execution\":%s,\"ack_unknown_revocation\":true,\"ack_no_hardware_admission\":true,\"ack_internal_tpm_accounting_unknown\":true,\"UNIT_TEST_MODEL_ONLY\":true}",p.request_hash,p.exe_hash,literal);C(n>0&&tj_parse(&p.approval,(size_t)n));return 1;}
typedef struct RACE {TP_PLATFORM platform;HANDLE start;int winner,error;} RACE;
static RACE races[6];static volatile LONG race_winners;
static DWORD WINAPI race_thread(void *arg){RACE *r=arg;wchar_t folder[MAX_PATH];wcscpy_s(folder,MAX_PATH,r->platform.folder);hp_init(&r->platform);wcscpy_s(r->platform.folder,MAX_PATH,folder);memset(r->platform.request_hash,'a',64);r->platform.request_hash[64]=0;memset(r->platform.approval_hash,'b',64);r->platform.approval_hash[64]=0;
 if(WaitForSingleObject(r->start,10000)!=WAIT_OBJECT_0){r->error=1;return 1;}r->winner=hp_receipt(&r->platform);if(r->winner)InterlockedIncrement(&race_winners);
 if(!tp_finish(&r->platform.proof)||!hp_close(&r->platform))r->error=1;return 0;
}
static int race(const wchar_t *base){wchar_t folder[MAX_PATH];HANDLE start,threads[6];unsigned i;
 C(swprintf_s(folder,MAX_PATH,L"%s\\receipt-race",base)>0&&CreateDirectoryW(folder,NULL));start=CreateEventW(NULL,TRUE,FALSE,NULL);C(start!=NULL);fault.close_mode=fault.write_mode=0;fault.owner=NULL;
 for(i=0;i<6;i++){C(swprintf_s(races[i].platform.folder,MAX_PATH,L"%s\\",folder)>0);races[i].start=start;threads[i]=CreateThread(NULL,0,race_thread,&races[i],0,NULL);C(threads[i]!=NULL);}
 C(SetEvent(start));for(i=0;i<6;i++){C(WaitForSingleObject(threads[i],10000)==WAIT_OBJECT_0);C(CloseHandle(threads[i]));C(!races[i].error);if(!races[i].winner)C(!strcmp(races[i].platform.proof.first,"ADMIN_RECEIPT_CREATE_NEW"));}C(race_winners==1);C(CloseHandle(start));puts("PASS SIX_CONCURRENT_RECEIPT_CREATORS EXACTLY_ONE_MODEL_WINNER NO_AUTHORITY");return 1;
}
static int test(const wchar_t *base,const wchar_t *cert){char *found,hex[65];unsigned i,length;wchar_t file[MAX_PATH],sub[MAX_PATH];DWORD wrote;HANDLE h;FILE *f=NULL;unsigned char data[32],expected[32];
 hp_init(&p);C(request_model());strcpy_s(canonical,TJ_BYTES,p.request.bytes);C(hp_request_contract(&p));
 {const char *needles[]={"\"direct_TBS_submit_maximum\":12","\"boot_record\":237900","native_own_sample_limit\":128","native_before_observer_reserve_limit\":124","shared_parent_console_sample_limit\":1024",TP_HOST_SHA,TP_EVENT_HOST_SHA,TP_OPERATOR,TP_BOOT_START,p.exe_hash,"C0-C9","7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985"};
 for(i=0;i<sizeof(needles)/sizeof(needles[0]);i++){tp_init(&p.proof);strcpy_s(p.request.bytes,TJ_BYTES,canonical);found=strstr(p.request.bytes,needles[i]);C(found!=NULL);found[strlen(needles[i])-1]^=1;C(tj_parse(&p.request,strlen(p.request.bytes)));C(!hp_request_contract(&p));}}
 tp_init(&p.proof);C(request_model());C(approval_model("true"));C(hp_approval_contract(&p));
 C(approval_model("false"));tp_init(&p.proof);C(!hp_approval_contract(&p));C(approval_model("\"true\""));tp_init(&p.proof);C(!hp_approval_contract(&p));
 C(approval_model("true"));p.approval.bytes[strstr(p.approval.bytes,"cccc")-p.approval.bytes]^=1;C(tj_parse(&p.approval,strlen(p.approval.bytes)));tp_init(&p.proof);C(!hp_approval_contract(&p));
 /* Real original-file reads: held FileID/share and exact whole content digest. */
 hp_init(&p);C(tp_crypto_init(&p.proof));C(swprintf_s(file,MAX_PATH,L"%s\\artifact.MODEL",base)>0);
 h=CreateFileW(file,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);C(h!=INVALID_HANDLE_VALUE);C(WriteFile(h,"fixture-original",16,&wrote,NULL)&&wrote==16&&FlushFileBuffers(h));C(CloseHandle(h));
 C(file_open(&p,0,file,1024,hex,data,sizeof(data),&length));C(length==16&&!memcmp(data,"fixture-original",16));C(tp_hash(&p.proof,(const unsigned char *)"fixture-original",16,expected));{char expectedHex[65];hp_hex(expected,32,expectedHex);C(!strcmp(hex,expectedHex));}
 C(hp_files_current(&p));h=CreateFileW(file,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);C(h==INVALID_HANDLE_VALUE&&GetLastError()==ERROR_SHARING_VIOLATION);
 p.files[0].id.nFileIndexLow^=1;C(!hp_files_current(&p)&&!strcmp(p.proof.first,"PINNED_FILE_CHANGED"));C(tp_finish(&p.proof));C(hp_close(&p));
 hp_init(&p);C(tp_crypto_init(&p.proof));{char wrong[65];memset(wrong,'d',64);wrong[64]=0;C(!hp_files(&p,wrong,wrong)&&!strcmp(p.proof.first,"EXE_PIN"));}C(tp_finish(&p.proof));C(hp_close(&p));
 /* Actual public DER parsing + CNG public-only import, no key creation on TPM. */
 hp_init(&p);C(tp_crypto_init(&p.proof));C(_wfopen_s(&f,cert,L"rb")==0&&f);p.files[3].bytes=(DWORD)fread(p.cert_bytes,1,sizeof(p.cert_bytes),f);C(p.files[3].bytes>0&&p.files[3].bytes<TP_CERT_MAX);C(fclose(f)==0);
 C(tp_hash(&p.proof,p.cert_bytes,p.files[3].bytes,expected));hp_hex(expected,32,hex);C(!strcmp(hex,TP_CERT_SHA));C(hp_load_certificate(&p));C(p.proof.public_key&&p.proof.modulus[0]);C(tp_finish(&p.proof));C(hp_close(&p));
 /* Exactly-once output and persistence failures in fresh synthetic folders. */
 for(i=0;i<5;i++){TP_PLATFORM *q=i==4?&retained:&p;LONG before;
 C(swprintf_s(sub,MAX_PATH,L"%s\\receipt-%u",base,i)>0&&CreateDirectoryW(sub,NULL));hp_init(q);C(swprintf_s(q->folder,MAX_PATH,L"%s\\",sub)>0);memset(q->request_hash,'a',64);memset(q->approval_hash,'b',64);q->request_hash[64]=q->approval_hash[64]=0;
 fault.owner=q;fault.write_mode=i==2?1:i==3?2:0;fault.close_mode=i==4;before=fault.creates;
 if(i==1){InterlockedExchange(&q->proof.stop,1);C(!hp_receipt(q)&&!q->receipt_created&&fault.creates==before);}
 else if(i==0){C(hp_receipt(q)&&q->receipt_created&&!q->receipt_uncertain&&!q->proof.controlled_refs);C(!hp_receipt(q)&&!strcmp(q->proof.first,"ADMIN_RECEIPT_CREATE_NEW"));}
 else{C(!hp_receipt(q)&&q->receipt_created);if(i==2)C(q->receipt_uncertain&&!strcmp(q->proof.first,"ADMIN_RECEIPT_WRITE")&&q->proof.first_native==ERROR_DISK_FULL);if(i==3)C(q->receipt_uncertain&&!strcmp(q->proof.first,"ADMIN_RECEIPT_SHORT_WRITE")&&!q->proof.first_native);if(i==4)C(q->proof.retained&&q->token&&!strcmp(q->proof.first,"WINDOWS_CLOSE_UNKNOWN"));}
 before=fault.creates;C(!hp_receipt(q)&&fault.creates==before);if(i!=4){C(tp_finish(&q->proof));C(hp_close(q));}
 }
 C(race(base));
 printf("PASS ARTIFACT assertions=%u original_file/JSON/pin/publicDER/oneuse/stop/shortwrite/first/retained; no TPM, no real approval created\n",checks);return 1;
}
int wmain(int argc,wchar_t **argv){if(argc!=3)return 2;return test(argv[1],argv[2])?0:1;}
