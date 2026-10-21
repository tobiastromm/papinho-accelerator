#include "../core/ed11_process_profile.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "c2_operation_win32.h"
#include <string.h>
/* Historical manager initialization only. No dispatch/Session/token implementation linked. */
PAPACC_RESULT papacc_c2_operation_manager_init(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_U32 owner,PAPACC_C2_OPEN_POLICY policy,PAPACC_C2_CAPACITY_GATE capacity,void*context){
 if(m==NULL||policy==NULL||capacity==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(m->initialized||owner!=(PAPACC_U32)GetCurrentThreadId()||sizeof(*m)>PAPACC_ED11_PROCESS_LIMIT)return PAPACC_RESULT_INVALID_STATE;
 memset(m,0,sizeof(*m));m->owner=owner;m->next_operation=1;m->policy=policy;m->capacity=capacity;m->context=context;m->initialized=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
