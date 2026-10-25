#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <string.h>
#include <stdio.h>
static unsigned first_count,end_count;static char output[16384];static size_t used;
static void trace_sink(const char *s){size_t n=strlen(s);assert(n<sizeof(output)-used);memcpy(output+used,s,n+1);used+=n;if(strstr(s,"PACC_PSR_FIRST_FAILURE"))first_count++;if(strstr(s,"PACC_PSR_DIAG_END"))end_count++;SetLastError(9876);}
#define OutputDebugStringA trace_sink
#define PAPACC_SCM_RUNTIME_LAB 1
#define PAPACC_SCM_REGISTRATION_LAB 1
#include "proposed/server_service_psr_trace_win32.inc"
static void reset(void){ZeroMemory(&psrt_record,sizeof(psrt_record));first_count=end_count=0;used=0;output[0]=0;}
int main(void){
 DWORD n;TOKEN_USER user;BYTE sid[12]={1,1,0,0,0,0,0,5,6,0,0,0};
 reset();PSRT_CONTEXT("MODEL_ONLY");SetLastError(5);assert(!PSRT_BOOL("OpenProcessToken",TOKEN_QUERY,FALSE));assert(GetLastError()==5);assert(PSRT_RETURN(5,"api")==5&&GetLastError()==5&&first_count==1&&end_count==1);assert(strstr(output,"raw_win32=5")&&strstr(output,"predicate=NONE"));assert(PSRT_RETURN(13,"secondary")==13&&first_count==1);
 reset();PSRT_CONTEXT("MODEL_ONLY");SetLastError(123);assert(PSRT_BOOL("query",0,TRUE));assert(!PSRT_PRED("enabled_group",FALSE));assert(PSRT_RETURN(5,"policy")==5&&GetLastError()==123&&strstr(output,"api_success=1 raw_win32=0")&&strstr(output,"predicate=enabled_group"));
 reset();SetLastError(88);assert(PSRT_NT("NtCreateFile",0x80120000,(LONG)0xc0000034L)==(LONG)0xc0000034L);assert(PSRT_RETURN(5,"existing_mapping")==5&&GetLastError()==88&&strstr(output,"nt_valid=1 ntstatus=c0000034"));
 reset();SetLastError(77);assert(PSRT_HANDLE("open",1,NULL)==NULL);assert(PSRT_STATUS("GetSecurityInfo",READ_CONTROL,5)==5&&GetLastError()==77);assert(PSRT_RETURN(5,"status")==5&&strstr(output,"raw_win32=5"));
 reset();SetLastError(22);assert(PSRT_COUNT("module",4)==4);PSRT_VALUES(1,2);assert(PSRT_RETURN(0,"success")==0&&first_count==0&&GetLastError()==22);
 reset();user.User.Sid=sid;user.User.Attributes=0;n=sizeof(user);assert(PSRT_TOKEN("GetTokenInformation:TokenUser",TokenUser,&user,sizeof(user),&n,TRUE));assert(psrt_record.token[0].length==sizeof(user));assert(PSRT_RETURN(5,"model_user")==5&&strstr(output,"token_user_bytes=")&&strstr(output,"UNAVAILABLE_OR_INVALID"));
 reset();n=100;assert(!PSRT_TOKEN("GetTokenInformation:TokenGroups",TokenGroups,&user,sizeof(user),&n,FALSE));assert(!psrt_record.token[1].length);
 reset();SetLastError(66);PSRT_CONTEXT("MODEL_ONLY");PSRT_PREPARED();assert(GetLastError()==66&&first_count==0&&strstr(output,"PACC_PSR_BEGIN")&&strstr(output,"PACC_PSR_PREPARE_SUCCESS"));
 assert(PSRT_STORAGE_BYTES==sizeof(PSRT_RECORD));printf("TRACE_RECORDER_MODEL_PASS; bytes=%lu; NO_SERVICE_TOKEN_OR_PERMISSION_FACTS\n",(unsigned long)PSRT_STORAGE_BYTES);return 0;
}
