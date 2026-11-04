/* Isolated resource feasibility only: NO TBS/TPM, admission, root or authority. */
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <winevt.h>
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
static int sample(const char *phase){DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n)){printf("QUERY_UNKNOWN %lu\n",GetLastError());return 0;}printf("%s=%lu\n",phase,n);return 1;}
static void retain(const char *where){printf("UNKNOWN_RETAINED %s\n",where);fflush(stdout);for(;;)Sleep(1000);}
int main(void){HANDLE token=NULL;TOKEN_STATISTICS stats;DWORD used=0,got=0;EVT_HANDLE query=NULL,event=NULL;BCRYPT_ALG_HANDLE alg=NULL;BCRYPT_HASH_HANDLE hash=NULL;unsigned char object[1024],digest[32];DWORD length=0;NTSTATUS status;
 if(!sample("entry"))return 1;
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return 1;
 if(!GetTokenInformation(token,TokenStatistics,&stats,sizeof(stats),&used)||used!=sizeof(stats)){retain("TOKEN_QUERY_FAILED");}
 if(!sample("token_held"))retain("TOKEN_METRIC_UNKNOWN");if(!CloseHandle(token))retain("TOKEN_CLOSE");token=NULL;if(!sample("after_identity"))return 1;
 query=EvtQuery(NULL,L"System",L"*[System[Provider[@Name='Microsoft-Windows-Kernel-General'] and EventID=12]]",EvtQueryChannelPath|EvtQueryReverseDirection);
 if(query){if(EvtNext(query,1,&event,0,0,&got)){if(!sample("event_held"))retain("EVENT_METRIC");if(!EvtClose(event))retain("EVENT_CLOSE");event=NULL;}else printf("event_read_error=%lu\n",GetLastError());if(!EvtClose(query))retain("QUERY_CLOSE");query=NULL;}
 else printf("event_query_error=%lu\n",GetLastError());
 if(!sample("after_eventlog"))return 1;
 status=BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,NULL,0);if(status<0)return 1;
 status=BCryptGetProperty(alg,BCRYPT_OBJECT_LENGTH,(PUCHAR)&length,sizeof(length),&got,0);if(status<0||got!=sizeof(length)||!length||length>sizeof(object))retain("HASH_SHAPE");
 status=BCryptCreateHash(alg,&hash,object,length,NULL,0,0);if(status<0)retain("HASH_CREATE");
 if(BCryptHashData(hash,(PUCHAR)"fixture-only",12,0)<0||BCryptFinishHash(hash,digest,sizeof(digest),0)<0)retain("HASH_QUERY");
 if(!sample("hash_held"))retain("HASH_METRIC");if(BCryptDestroyHash(hash)<0)retain("HASH_CLOSE");hash=NULL;if(BCryptCloseAlgorithmProvider(alg,0)<0)retain("PROVIDER_CLOSE");alg=NULL;
 SecureZeroMemory(object,sizeof(object));if(!sample("after_hash"))return 1;puts("PASS NATIVE_RESOURCE_PHASES_ONLY NO_TPM_NO_ADMISSION NO_FULL_PROOF_CAPACITY");return 0;
}
