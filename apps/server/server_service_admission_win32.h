#ifndef PAPACC_SERVICE_ADMISSION_WIN32_H
#define PAPACC_SERVICE_ADMISSION_WIN32_H
/* Private single-LAB specialization of the existing SCM owner; no authority. */
#include "server_service_admission_bd_win32.h"
typedef struct PS_ADMISSION {
 HANDLE mutex,intent,launch,launcher;DWORD thread;unsigned phase,owned,consumed,effects,stop,lab;
 PSECURITY_DESCRIPTOR pending,expected;HANDLE token;
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 PS_BD_STATE bd;
#endif
} PS_ADMISSION;
DWORD papacc_server_service_admission_bind_launch(DWORD,unsigned __int64);
DWORD papacc_server_service_admission_enter(HANDLE,const char *);
DWORD papacc_server_service_admission_consume(void);
DWORD papacc_server_service_admission_effect(void);
DWORD papacc_server_service_admission_stop(void);
DWORD papacc_server_service_admission_finish(int);
void papacc_server_service_admission_retain(void);
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
DWORD papacc_server_service_admission_bd_bind(const PS_BD_DESCRIPTOR *,PS_BD_CONTEXT_CHECK);
DWORD papacc_server_service_admission_bd_revalidate(PS_BD_CONTEXT_CHECK);
DWORD papacc_server_service_admission_bd_close(int quiescent);
DWORD papacc_server_service_admission_bd_metrics(const PS_BD_COST *,ULONGLONG);
#endif
#endif
