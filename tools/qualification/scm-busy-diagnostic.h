#ifndef PAPACC_SCM_BUSY_DIAGNOSTIC_H
#define PAPACC_SCM_BUSY_DIAGNOSTIC_H
/* Qualifier-only first-failure metadata; no authority, worker or owned ref. */
#include <windows.h>
#include <stdio.h>
typedef struct IG_DIAGNOSTIC {
 const char *stage,*api,*resource;DWORD code,native,pid,role;
 unsigned present,current_role;DWORD current_pid;
} IG_DIAGNOSTIC;
static IG_DIAGNOSTIC ig_diagnostic;
static __inline void ig_diag_note(const char *stage,const char *api,const char *resource,DWORD code,DWORD native_result,DWORD pid,unsigned role)
{
 DWORD saved=GetLastError();
 if(!ig_diagnostic.present){
  ig_diagnostic.present=1;ig_diagnostic.stage=stage;ig_diagnostic.api=api;ig_diagnostic.resource=resource;
  ig_diagnostic.code=code;ig_diagnostic.native=native_result;ig_diagnostic.pid=pid;ig_diagnostic.role=role;
  printf("IG_DIAGNOSTIC stage=%s api=%s resource=%s code=%lu native=%lu pid=%lu role=%u; FIRST_FAILURE_ONLY; NO_AUTHORITY\n",stage,api,resource,(unsigned long)code,(unsigned long)native_result,(unsigned long)pid,role);
 }
 SetLastError(saved);
}
static __inline BOOL ig_diag_bool(const char *stage,const char *api,const char *resource,BOOL value)
{DWORD e=GetLastError();if(!value)ig_diag_note(stage,api,resource,e,e,ig_diagnostic.current_pid,ig_diagnostic.current_role);SetLastError(e);return value;}
static __inline HANDLE ig_diag_handle(const char *stage,const char *api,const char *resource,HANDLE value)
{DWORD e=GetLastError();if(!value||value==INVALID_HANDLE_VALUE)ig_diag_note(stage,api,resource,e,e,ig_diagnostic.current_pid,ig_diagnostic.current_role);SetLastError(e);return value;}
static __inline SIZE_T ig_diag_size(const char *stage,const char *api,const char *resource,SIZE_T value)
{DWORD e=GetLastError();if(!value)ig_diag_note(stage,api,resource,e,e,ig_diagnostic.current_pid,ig_diagnostic.current_role);SetLastError(e);return value;}
static __inline DWORD ig_diag_status(const char *stage,const char *api,const char *resource,DWORD value,int end_allowed)
{DWORD e=GetLastError();if(value&&!(end_allowed&&value==ERROR_NO_MORE_ITEMS))ig_diag_note(stage,api,resource,value,value,ig_diagnostic.current_pid,ig_diagnostic.current_role);SetLastError(e);return value;}
static __inline DWORD ig_diag_wait_live(const char *stage,const char *api,const char *resource,DWORD value)
{DWORD e=GetLastError();if(value==WAIT_FAILED)ig_diag_note(stage,api,resource,e,value,ig_diagnostic.current_pid,ig_diagnostic.current_role);SetLastError(e);return value;}
static __inline BOOL ig_diag_next(const char *stage,const char *api,const char *resource,BOOL value)
{DWORD e=GetLastError();if(!value&&e!=ERROR_NO_MORE_FILES)ig_diag_note(stage,api,resource,e,e,ig_diagnostic.current_pid,ig_diagnostic.current_role);SetLastError(e);return value;}
#endif
