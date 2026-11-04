/* Native side effects fully substituted; exercise the production FCR helper. */
static unsigned prepared_fault,prepared_opens,prepared_closes,prepared_publics,prepared_scm_queries;
static SC_HANDLE WINAPI prepared_model_manager(LPCWSTR machine,LPCWSTR database,DWORD access)
{if(machine||database||access!=SC_MANAGER_CONNECT)return NULL;return (SC_HANDLE)(ULONG_PTR)901;}
static SC_HANDLE WINAPI prepared_model_service(SC_HANDLE manager,LPCWSTR name,DWORD access)
{if(manager!=(SC_HANDLE)(ULONG_PTR)901||wcscmp(name,L"PapinhoAccelerator")||access!=(SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS))return NULL;return (SC_HANDLE)(ULONG_PTR)902;}
static BOOL WINAPI prepared_model_config(SC_HANDLE service,LPQUERY_SERVICE_CONFIGW q,DWORD capacity,LPDWORD needed)
{
 WCHAR *text=(WCHAR *)((BYTE *)q+sizeof(*q));size_t remaining=(capacity-sizeof(*q))/sizeof(WCHAR),z;
 (void)service;prepared_scm_queries++;memset(q,0,capacity);*needed=0;
 q->dwServiceType=SERVICE_WIN32_OWN_PROCESS;q->dwStartType=SERVICE_DEMAND_START;q->dwErrorControl=SERVICE_ERROR_NORMAL;
 q->lpBinaryPathName=text;wcscpy_s(text,remaining,L"\"C:\\Program Files\\Papinho\\Accelerator\\papacc_server.exe\" service host scq-closed");z=wcslen(text)+1U;text+=z;remaining-=z;
 q->lpServiceStartName=text;wcscpy_s(text,remaining,L"NT SERVICE\\PapinhoAccelerator");z=wcslen(text)+1U;text+=z;
 q->lpDependencies=q->lpLoadOrderGroup=text;*text=0;
 if(prepared_fault==1U)q->dwStartType=SERVICE_AUTO_START;return TRUE;
}
static BOOL WINAPI prepared_model_sid(SC_HANDLE h,DWORD type,LPBYTE value,DWORD bytes,LPDWORD needed)
{(void)h;if(type!=SERVICE_CONFIG_SERVICE_SID_INFO||bytes!=sizeof(SERVICE_SID_INFO))return FALSE;((SERVICE_SID_INFO *)value)->dwServiceSidType=SERVICE_SID_TYPE_UNRESTRICTED;*needed=0;return TRUE;}
static BOOL WINAPI prepared_model_status(SC_HANDLE h,SC_STATUS_TYPE type,LPBYTE value,DWORD bytes,LPDWORD needed)
{SERVICE_STATUS_PROCESS *s=(SERVICE_STATUS_PROCESS *)value;(void)h;if(type!=SC_STATUS_PROCESS_INFO||bytes!=sizeof(*s))return FALSE;memset(s,0,bytes);s->dwServiceType=SERVICE_WIN32_OWN_PROCESS;s->dwCurrentState=prepared_fault==3U?SERVICE_START_PENDING:SERVICE_STOPPED;*needed=bytes;return TRUE;}
static BOOL WINAPI prepared_model_close_service(SC_HANDLE h)
{if(h!=(SC_HANDLE)(ULONG_PTR)901&&h!=(SC_HANDLE)(ULONG_PTR)902)return FALSE;prepared_closes++;return prepared_fault==5U?FALSE:TRUE;}
static DWORD WINAPI prepared_model_module(HMODULE h,LPWSTR out,DWORD capacity)
{(void)h;wcscpy_s(out,capacity,prepared_fault==2U?L"C:\\Other\\papacc_server.exe":L"C:\\Program Files\\Papinho\\Accelerator\\papacc_server.exe");return (DWORD)wcslen(out);}
static HMODULE WINAPI prepared_model_module_handle(LPCWSTR name){(void)name;return (HMODULE)(ULONG_PTR)1;}
static BOOL WINAPI prepared_model_seek(HANDLE h,LARGE_INTEGER at,PLARGE_INTEGER next,DWORD method)
{(void)next;return h==(HANDLE)(ULONG_PTR)501&&!at.QuadPart&&method==FILE_BEGIN;}
static BOOL WINAPI prepared_model_read(HANDLE h,LPVOID out,DWORD capacity,LPDWORD n,LPOVERLAPPED overlapped)
{(void)overlapped;if(h!=(HANDLE)(ULONG_PTR)501||capacity!=8U)return FALSE;memset(out,0x41,capacity);*n=capacity;return TRUE;}
static NTSTATUS WINAPI prepared_model_algorithm(BCRYPT_ALG_HANDLE *h,LPCWSTR alg,LPCWSTR implementation,ULONG flags)
{if(wcscmp(alg,BCRYPT_SHA256_ALGORITHM)||implementation||flags)return (NTSTATUS)0xc000000dL;*h=(BCRYPT_ALG_HANDLE)(ULONG_PTR)1;return 0;}
static NTSTATUS WINAPI prepared_model_hash(BCRYPT_ALG_HANDLE a,BCRYPT_HASH_HANDLE *h,PUCHAR object,ULONG bytes,PUCHAR secret,ULONG secret_bytes,ULONG flags)
{(void)a;(void)object;(void)bytes;(void)secret;(void)secret_bytes;(void)flags;*h=(BCRYPT_HASH_HANDLE)(ULONG_PTR)2;return 0;}
static NTSTATUS WINAPI prepared_model_update(BCRYPT_HASH_HANDLE h,PUCHAR bytes,ULONG n,ULONG flags)
{(void)h;(void)bytes;(void)n;(void)flags;return 0;}
static NTSTATUS WINAPI prepared_model_finish(BCRYPT_HASH_HANDLE h,PUCHAR out,ULONG bytes,ULONG flags)
{(void)h;(void)flags;if(bytes!=32U)return (NTSTATUS)0xc000000dL;memset(out,0x71,bytes);return 0;}
static DWORD personal_scq_contents_cleanup(PAPACC_PERSONAL_SCQ_INPUTS *c)
{c->content_hash.hash=NULL;c->content_hash.algorithm=NULL;c->content_hash.active=0;return 0;}
static int personal_scq_inputs_sd_policy(PSECURITY_DESCRIPTOR sd,PSID actor,int image)
{(void)sd;(void)actor;return image&&sd_fault==0U;}
static SECURITY_STATUS prepared_model_key_open(PAPACC_CA_KEY *key,const BYTE a[16],const BYTE g[16],PSID actor,PSID service)
{(void)a;(void)g;(void)actor;(void)service;prepared_opens++;key->state=PAPACC_CA_KEY_READY;return prepared_fault==4U?NTE_BAD_KEYSET:0;}
static SECURITY_STATUS prepared_model_key_public(PAPACC_CA_KEY *key,BYTE *blob,DWORD capacity,DWORD *n)
{(void)key;prepared_publics++;if(capacity<283U)return NTE_BUFFER_TOO_SMALL;memset(blob,0x41,283);*n=283;return 0;}
static int prepared_model_digest(PAPACC_STORE_HASH *hash,const BYTE *bytes,unsigned n,BYTE out[32])
{(void)hash;(void)bytes;(void)n;memset(out,0x72,32);return 1;}
#define OpenSCManagerW prepared_model_manager
#define OpenServiceW prepared_model_service
#define QueryServiceConfigW prepared_model_config
#define QueryServiceConfig2W prepared_model_sid
#define QueryServiceStatusEx prepared_model_status
#define GetModuleFileNameW prepared_model_module
#define GetModuleHandleW prepared_model_module_handle
#define SetFilePointerEx prepared_model_seek
#define ReadFile prepared_model_read
#define BCryptOpenAlgorithmProvider prepared_model_algorithm
#define BCryptCreateHash prepared_model_hash
#define BCryptHashData prepared_model_update
#define BCryptFinishHash prepared_model_finish
#define papacc_ca_key_prepared_read_open prepared_model_key_open
#define papacc_ca_key_prepared_public prepared_model_key_public
#define papacc_store_hash prepared_model_digest
#define papacc_configuration_personal_prepared_plan prepared_plan_model
#define papacc_configuration_personal_prepared_validate prepared_validate_model
#include "../apps/server/server_configuration_prepared_validate_win32.inc"
#undef papacc_store_hash
#undef papacc_ca_key_prepared_read_open
#undef papacc_ca_key_prepared_public
