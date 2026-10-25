#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static struct {WCHAR command[1050];struct {struct {DWORD dwErrorControl;} aligned;} query;} native;
static const WCHAR cycle_path[]=L"C:\\fixture\\new10584.exe";
static const WCHAR *observed;static DWORD injected;static int calls,append_calls,fail_append;
static int lab_append(WCHAR *out,size_t cap,const WCHAR *text){size_t n=wcslen(out),m=wcslen(text);if(++append_calls==fail_append||n>=cap||m>=cap-n)return 0;memcpy(out+n,text,(m+1)*sizeof(WCHAR));return 1;}
static DWORD ps_virtual_readback_start(DWORD startup){calls++;if(injected)return injected;if(startup!=SERVICE_DEMAND_START||wcscmp(native.command,observed))return ERROR_INVALID_DATA;return 0;}
#define T(x) do{cases++;if(!(x)){printf("FAIL %u line%d\n",cases,__LINE__);return 1;}}while(0)
/* Exact preserved historical predecessor; never adopt current SCM text. */
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
static const WCHAR cycle_previous_image[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe";
#endif
static DWORD cycle_readback(DWORD startup,int candidate)
{
 WCHAR saved[1050];DWORD e;const WCHAR *expected=NULL;memcpy(saved,native.command,sizeof(saved));
 if(candidate)expected=cycle_path;
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
 else expected=cycle_previous_image;
#endif
 if(expected){native.command[0]=0;if(!lab_append(native.command,1050,L"\"")||!lab_append(native.command,1050,expected)||!lab_append(native.command,1050,L"\" service host")){memcpy(native.command,saved,sizeof(saved));return ERROR_INVALID_DATA;}}
 e=ps_virtual_readback_start(startup);
 memcpy(native.command,saved,sizeof(saved));
 if(!e&&native.query.aligned.dwErrorControl!=SERVICE_ERROR_NORMAL)e=ERROR_INVALID_DATA;
 return e;
}
int main(void){unsigned cases=0;int i;WCHAR preserved[1050];
 wcscpy_s(native.command,1050,L"\"C:\\old-generic.exe\" service host");memcpy(preserved,native.command,sizeof(preserved));native.query.aligned.dwErrorControl=SERVICE_ERROR_NORMAL;
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
 observed=L"\"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe\" service host";
#else
 observed=preserved;
#endif
 T(cycle_readback(SERVICE_DEMAND_START,0)==0);T(!memcmp(native.command,preserved,sizeof(preserved)));T(calls==1);
 observed=L"\"C:\\fixture\\new10584.exe\" service host";T(cycle_readback(SERVICE_DEMAND_START,1)==0);T(!memcmp(native.command,preserved,sizeof(preserved)));
 observed=L"\"C:\\other.exe\" service host";T(cycle_readback(SERVICE_DEMAND_START,0)==ERROR_INVALID_DATA);T(!memcmp(native.command,preserved,sizeof(preserved)));
 injected=ERROR_ACCESS_DENIED;T(cycle_readback(SERVICE_DEMAND_START,0)==ERROR_ACCESS_DENIED);injected=0;
 observed=L"\"C:\\fixture\\new10584.exe\" service host";native.query.aligned.dwErrorControl=2;T(cycle_readback(SERVICE_DEMAND_START,1)==ERROR_INVALID_DATA);native.query.aligned.dwErrorControl=SERVICE_ERROR_NORMAL;
 for(i=1;i<=3;i++){append_calls=0;fail_append=i;T(cycle_readback(SERVICE_DEMAND_START,1)==ERROR_INVALID_DATA);T(!memcmp(native.command,preserved,sizeof(preserved)));}fail_append=0;
 T(cycle_readback(SERVICE_DISABLED,1)==ERROR_INVALID_DATA);T(!memcmp(native.command,preserved,sizeof(preserved)));
 printf("ACTUAL_CYCLE_READBACK=PASS cases=%u; MOCK_SCM_ONLY_NO_HANDLES\n",cases);return 0;}
