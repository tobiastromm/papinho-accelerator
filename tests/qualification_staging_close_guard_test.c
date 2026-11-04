#include "server_configuration_commit_win32.h"
#include <stdio.h>
static PAPACC_CONFIGURATION_COMMIT owner;
int main(void)
{
 unsigned i;static const unsigned live[]={1,2,3,4,7,8};
 owner.writer=GetCurrentThreadId();
 for(i=0;i<sizeof(live)/sizeof(live[0]);i++){
  owner.pending_commit.personal_native.staging.phase=live[i];
  if(papacc_configuration_personal_native_close(&owner)!=PAPACC_RESULT_INVALID_STATE)return 1;
 }
 owner.pending_commit.personal_native.staging.phase=1U;
 if(papacc_configuration_personal_layout_binary(&owner)!=PAPACC_RESULT_INVALID_STATE)return 2;
 owner.pending_commit.personal_native.staging.self=&owner.pending_commit.personal_native.staging;
 owner.pending_commit.personal_native.staging.thread=GetCurrentThreadId();
 if(papacc_configuration_personal_layout_binary(&owner)!=PAPACC_RESULT_INVALID_STATE)return 3;
 owner.pending_commit.personal_native.staging.invocation=1U;
 owner.pending_commit.personal_native.install_paths.thread=GetCurrentThreadId();
 owner.pending_commit.personal_native.install_paths.phase=PAPACC_PERSONAL_PATHS_READY;
 owner.pending_commit.personal_native.install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;
 if(papacc_configuration_personal_layout_binary_current(&owner)!=PAPACC_RESULT_INVALID_STATE)return 4;
 owner.pending_commit.personal_native.install_paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;
 if(papacc_configuration_personal_layout_current(&owner)!=PAPACC_RESULT_INVALID_STATE)return 5;
 owner.pending_commit.personal_native.staging.phase=0U;
 owner.pending_commit.personal_native.staged_update.phase=1U;
 if(papacc_configuration_personal_native_close(&owner)!=PAPACC_RESULT_INVALID_STATE)return 6;
 owner.pending_commit.personal_native.staged_update.phase=8U;
 if(papacc_configuration_personal_native_close(&owner)!=PAPACC_RESULT_INVALID_STATE)return 7;
 puts("PASS STAGING_NATIVE_CLOSE_GUARD six live/retained states; no acquisition/close/IO performed");return 0;
}
