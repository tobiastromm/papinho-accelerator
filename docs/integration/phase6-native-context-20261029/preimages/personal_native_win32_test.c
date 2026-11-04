#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT model;
int main(void)
{
 char path[MAX_PATH];DWORD n;PAPACC_RESULT r;
 model.writer=GetCurrentThreadId();
 if(papacc_configuration_personal_native_current(&model)==PAPACC_RESULT_OK)return 1;
 n=GetTempPathA(MAX_PATH,path);if(!n||n>=MAX_PATH)return 2;
 r=papacc_configuration_personal_native_open(&model,path);
 if(r!=PAPACC_RESULT_OK){
 if(papacc_configuration_personal_native_close(&model)!=PAPACC_RESULT_OK)return 3;
 puts("NATIVE_READ_ONLY_PREFLIGHT_DENIED_NO_ELIGIBLE_CONTEXT_OR_PARENT; POSITIVE_NOT_QUALIFIED");return 0;}
 if(!model.pending_commit.bootstrap_parent||!model.pending_commit.bootstrap_token||model.pending_commit.bootstrap_created||
 !model.pending_commit.personal_native.sid_bytes||papacc_configuration_personal_native_current(&model)!=PAPACC_RESULT_OK)return 4;
 if(papacc_configuration_personal_native_open(&model,path)==PAPACC_RESULT_OK)return 5;
 model.stopped=1;if(papacc_configuration_personal_native_current(&model)==PAPACC_RESULT_OK)return 6;
 if(papacc_configuration_personal_native_close(&model)!=PAPACC_RESULT_OK)return 7;
 if(model.pending_commit.bootstrap_parent||model.pending_commit.bootstrap_token)return 8;
 puts("NATIVE_READ_ONLY_PREFLIGHT_ELIGIBLE_PARENT_TOKEN_REVALIDATED_CLOSED_NO_CREATION");return 0;
}
