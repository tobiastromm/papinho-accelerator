#ifdef NDEBUG
#undef NDEBUG
#endif
/* Isolated wait/retirement model of SAME LAB driver. Never calls cycle entry. */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <assert.h>
static DWORD model_states[4],model_count,model_next,model_error,model_size;static ULONGLONG model_tick;static unsigned model_sleeps;
static BOOL WINAPI model_status(SC_HANDLE h,SC_STATUS_TYPE kind,LPBYTE out,DWORD cap,LPDWORD used)
{
 SERVICE_STATUS_PROCESS *s=(SERVICE_STATUS_PROCESS *)out;(void)h;(void)kind;assert(cap==sizeof(*s));
 if(model_error){SetLastError(model_error);return FALSE;}
 ZeroMemory(s,sizeof(*s));s->dwServiceType=SERVICE_WIN32_OWN_PROCESS;s->dwCurrentState=model_states[model_next<model_count?model_next++:model_count-1];*used=model_size;return TRUE;
}
static ULONGLONG WINAPI model_clock(void){ULONGLONG n=model_tick;model_tick+=16000;return n;}
static VOID WINAPI model_sleep(DWORD ms){assert(ms==50);model_sleeps++;}
#define QueryServiceStatusEx model_status
#define GetTickCount64 model_clock
#define Sleep model_sleep
#define PAPACC_SCM_CYCLE_LAB 1
#define main unused_cycle_entry
#include "../tools/qualification/scm-registration-lab.c"
#undef main
static void reset_model(DWORD a,DWORD b)
{model_states[0]=a;model_states[1]=b;model_count=2;model_next=0;model_error=0;model_size=sizeof(SERVICE_STATUS_PROCESS);model_tick=0;model_sleeps=0;}
int main(void)
{
 SERVICE_STATUS_PROCESS s;
 reset_model(SERVICE_START_PENDING,SERVICE_RUNNING);assert(cycle_wait(SERVICE_RUNNING,SERVICE_START_PENDING,&s)==0&&s.dwCurrentState==SERVICE_RUNNING&&model_sleeps==1);
 reset_model(SERVICE_STOP_PENDING,SERVICE_STOPPED);assert(cycle_wait(SERVICE_STOPPED,SERVICE_STOP_PENDING,&s)==0&&s.dwCurrentState==SERVICE_STOPPED);
 cycle_stop_requested=1;reset_model(SERVICE_RUNNING,SERVICE_STOPPED);assert(cycle_wait(SERVICE_STOPPED,SERVICE_STOP_PENDING,&s)==0);cycle_stop_requested=0;
 reset_model(SERVICE_START_PENDING,SERVICE_START_PENDING);assert(cycle_wait(SERVICE_RUNNING,SERVICE_START_PENDING,&s)==ERROR_TIMEOUT&&s.dwCurrentState==SERVICE_START_PENDING);
 reset_model(SERVICE_STOP_PENDING,SERVICE_STOP_PENDING);assert(cycle_wait(SERVICE_STOPPED,SERVICE_STOP_PENDING,&s)==ERROR_TIMEOUT&&s.dwCurrentState==SERVICE_STOP_PENDING);
 reset_model(SERVICE_STOPPED,SERVICE_STOPPED);assert(cycle_wait(SERVICE_RUNNING,SERVICE_START_PENDING,&s)==ERROR_INVALID_STATE);
 reset_model(SERVICE_RUNNING,SERVICE_RUNNING);model_size=1;assert(cycle_wait(SERVICE_RUNNING,SERVICE_START_PENDING,&s)==ERROR_INVALID_DATA);
 reset_model(SERVICE_RUNNING,SERVICE_RUNNING);model_error=ERROR_ACCESS_DENIED;assert(cycle_wait(SERVICE_RUNNING,SERVICE_START_PENDING,&s)==ERROR_ACCESS_DENIED);
 cycle_start_requested=0;cycle_quiescent=0;assert(cycle_can_retire());cycle_start_requested=1;assert(!cycle_can_retire());cycle_stop_requested=1;assert(!cycle_can_retire());cycle_quiescent=1;assert(cycle_can_retire());
 native.retained=1;cycle_file=(HANDLE)(ULONG_PTR)1;assert(cycle_cleanup()==ERROR_INVALID_STATE&&cycle_file==(HANDLE)(ULONG_PTR)1);cycle_file=NULL;native.retained=0;
 puts("LAB_CYCLE_WAIT_AND_RETIREMENT_MODEL_PASS; NO_SERVICE_OR_FILE_EFFECTS");return 0;
}
