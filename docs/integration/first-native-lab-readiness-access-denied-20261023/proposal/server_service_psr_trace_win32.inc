/* PROPOSED private LAB-only first-failure recorder. No authority or handles. */
#if !defined(PAPACC_SCM_RUNTIME_LAB) || !defined(PAPACC_SCM_REGISTRATION_LAB)
#error PSR tracing is restricted to the fixed private LAB profile
#endif
#include <stdio.h>
typedef struct PSRT_BORROW {void *data;DWORD length,capacity;} PSRT_BORROW;
typedef struct PSRT_RECORD {
 PSRT_BORROW token[3];const char *object,*api,*predicate;DWORD requested,raw,returned,line,value_a,value_b;LONG ntstatus;unsigned native_valid,success,emitted,began,prepared;
} PSRT_RECORD;
static PSRT_RECORD psrt_record;
#define PSRT_STORAGE_BYTES sizeof(psrt_record)
static void psrt_context(const char *tag){DWORD saved=GetLastError();char line[192];psrt_record.object=tag;psrt_record.predicate=NULL;
 if(!psrt_record.began){psrt_record.began=1;(void)_snprintf_s(line,sizeof(line),_TRUNCATE,"PACC_PSR_BEGIN pid=%lu tid=%lu private_lab=1\n",(unsigned long)GetCurrentProcessId(),(unsigned long)GetCurrentThreadId());OutputDebugStringA(line);}SetLastError(saved);}
static void psrt_prepared(void){DWORD saved=GetLastError();char line[192];if(!psrt_record.prepared){psrt_record.prepared=1;(void)_snprintf_s(line,sizeof(line),_TRUNCATE,"PACC_PSR_PREPARE_SUCCESS pid=%lu NOT_OPERATIONAL_ADMISSION\n",(unsigned long)GetCurrentProcessId());OutputDebugStringA(line);}SetLastError(saved);}
static BOOL psrt_bool(const char *name,unsigned line,DWORD requested,BOOL value)
{DWORD saved=GetLastError();psrt_record.api=name;psrt_record.line=line;psrt_record.requested=requested;psrt_record.raw=value?0:saved;psrt_record.success=value!=FALSE;psrt_record.native_valid=0;SetLastError(saved);return value;}
static HANDLE psrt_handle(const char *name,unsigned line,DWORD requested,HANDLE value)
{(void)psrt_bool(name,line,requested,value&&value!=INVALID_HANDLE_VALUE);return value;}
static DWORD psrt_status(const char *name,unsigned line,DWORD requested,DWORD value)
{DWORD saved=GetLastError();(void)psrt_bool(name,line,requested,value==0);psrt_record.raw=value;SetLastError(saved);return value;}
static DWORD psrt_count(const char *name,unsigned line,DWORD value)
{(void)psrt_bool(name,line,0,value!=0);return value;}
static LONG psrt_nt(const char *name,unsigned line,DWORD requested,LONG value)
{DWORD saved=GetLastError();(void)psrt_bool(name,line,requested,value>=0);psrt_record.ntstatus=value;psrt_record.native_valid=1;psrt_record.raw=0;SetLastError(saved);return value;}
static BOOL psrt_pred(const char *name,BOOL value){if(!value)psrt_record.predicate=name;return value;}
static void psrt_values(DWORD a,DWORD b){psrt_record.value_a=a;psrt_record.value_b=b;}
static BOOL psrt_token(const char *name,unsigned line,TOKEN_INFORMATION_CLASS cls,void *data,DWORD capacity,DWORD *length,BOOL value)
{
 unsigned slot=3;(void)psrt_bool(name,line,TOKEN_QUERY,value);
 if(cls==TokenUser)slot=0;else if(cls==TokenGroups)slot=1;else if(cls==TokenPrivileges)slot=2;
 if(value&&length&&*length<=capacity&&slot<3){psrt_record.token[slot].data=data;psrt_record.token[slot].length=*length;psrt_record.token[slot].capacity=capacity;}
 return value;
}
static const char *psrt_sid_hex(const PSRT_BORROW *b,PSID sid,char out[192])
{
 ULONG_PTR start=(ULONG_PTR)b->data,p=(ULONG_PTR)sid;DWORD n,i;
 if(!b->data||b->length>b->capacity||p<start||p-start>b->length||b->length-(DWORD)(p-start)<8)return "UNAVAILABLE_OR_INVALID";
 n=8U+4U*((BYTE *)sid)[1];if(n>68||n>b->length-(DWORD)(p-start))return "UNAVAILABLE_OR_INVALID";
 for(i=0;i<n;i++){out[i*2]="0123456789abcdef"[((BYTE *)sid)[i]>>4];out[i*2+1]="0123456789abcdef"[((BYTE *)sid)[i]&15];}out[n*2]=0;return out;
}
static void psrt_token_emit(void)
{
 char line[384],sid[192];PSRT_BORROW *b;DWORD i,count;
 b=&psrt_record.token[0];if(b->data&&b->length>=sizeof(TOKEN_USER)){
 (void)_snprintf_s(line,sizeof(line),_TRUNCATE,"PACC_PSR_TOKEN_USER pid=%lu sid_hex=%s\n",(unsigned long)GetCurrentProcessId(),psrt_sid_hex(b,((TOKEN_USER *)b->data)->User.Sid,sid));OutputDebugStringA(line);}
 b=&psrt_record.token[1];if(b->data&&b->length>=(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)){
 TOKEN_GROUPS *g=(TOKEN_GROUPS *)b->data;count=g->GroupCount;
 if(count<=(b->length-FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))for(i=0;i<count;i++){
 (void)_snprintf_s(line,sizeof(line),_TRUNCATE,"PACC_PSR_TOKEN_GROUP pid=%lu index=%lu sid_hex=%s attributes=%08lx\n",(unsigned long)GetCurrentProcessId(),(unsigned long)i,psrt_sid_hex(b,g->Groups[i].Sid,sid),(unsigned long)g->Groups[i].Attributes);OutputDebugStringA(line);}}
 b=&psrt_record.token[2];if(b->data&&b->length>=(DWORD)FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges)){
 TOKEN_PRIVILEGES *v=(TOKEN_PRIVILEGES *)b->data;count=v->PrivilegeCount;
 if(count<=(b->length-FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges))/sizeof(LUID_AND_ATTRIBUTES))for(i=0;i<count;i++){
 (void)_snprintf_s(line,sizeof(line),_TRUNCATE,"PACC_PSR_TOKEN_PRIVILEGE pid=%lu index=%lu luid=%ld:%lu attributes=%08lx\n",(unsigned long)GetCurrentProcessId(),(unsigned long)i,(long)v->Privileges[i].Luid.HighPart,(unsigned long)v->Privileges[i].Luid.LowPart,(unsigned long)v->Privileges[i].Attributes);OutputDebugStringA(line);}}
 (void)_snprintf_s(line,sizeof(line),_TRUNCATE,"PACC_PSR_DIAG_END pid=%lu token_user_bytes=%lu token_group_bytes=%lu token_privilege_bytes=%lu absent_buffers=NOT_REACHED_NOT_SYNTHETIC\n",(unsigned long)GetCurrentProcessId(),(unsigned long)psrt_record.token[0].length,(unsigned long)psrt_record.token[1].length,(unsigned long)psrt_record.token[2].length);OutputDebugStringA(line);
}
static DWORD psrt_return(DWORD value,unsigned line,const char *branch)
{
 DWORD saved=GetLastError();char text[768];int n;
 if(value&&!psrt_record.emitted){
 psrt_record.emitted=1;psrt_record.returned=value;
 n=_snprintf_s(text,sizeof(text),_TRUNCATE,"PACC_PSR_FIRST_FAILURE pid=%lu tid=%lu object=%s api=%s api_line=%lu request=%08lx api_success=%u raw_win32=%lu nt_valid=%u ntstatus=%08lx predicate=%s branch=%s return_line=%u returned_win32=%lu observed_a=%lu observed_b=%lu token_handle=NOT_EXPORTED\n",
 (unsigned long)GetCurrentProcessId(),(unsigned long)GetCurrentThreadId(),psrt_record.object?psrt_record.object:"UNSET",psrt_record.api?psrt_record.api:"NONE",(unsigned long)psrt_record.line,(unsigned long)psrt_record.requested,psrt_record.success,(unsigned long)psrt_record.raw,psrt_record.native_valid,(unsigned long)(DWORD)psrt_record.ntstatus,psrt_record.predicate?psrt_record.predicate:"NONE",branch,line,(unsigned long)value,(unsigned long)psrt_record.value_a,(unsigned long)psrt_record.value_b);
 if(n>0)OutputDebugStringA(text);
 psrt_token_emit();
 }
 SetLastError(saved);return value;
}
#define PSRT_CONTEXT(tag) psrt_context(tag)
#define PSRT_BOOL(tag,request,expr) psrt_bool(tag,__LINE__,request,(expr))
#define PSRT_HANDLE(tag,request,expr) psrt_handle(tag,__LINE__,request,(expr))
#define PSRT_STATUS(tag,request,expr) psrt_status(tag,__LINE__,request,(expr))
#define PSRT_COUNT(tag,expr) psrt_count(tag,__LINE__,(expr))
#define PSRT_NT(tag,request,expr) psrt_nt(tag,__LINE__,request,(expr))
#define PSRT_PRED(tag,expr) psrt_pred(tag,(expr))
#define PSRT_RETURN(value,branch) psrt_return((value),__LINE__,branch)

#define PSRT_VALUES(a,b) psrt_values(a,b)
#define PSRT_TOKEN(tag,cls,buf,capacity,length,expr) psrt_token(tag,__LINE__,cls,buf,capacity,length,(expr))

#define PSRT_PREPARED() psrt_prepared()
