#define _WIN32_WINNT 0x0600
#include "personal_scm_registration_win32.h"
#include <psapi.h>
#include <string.h>
#include "server_personal_resource_sample_win32.inc"
/* One prospective sampler invocation, never the FCA owner or an authority. */
int main(void)
{
 PAPACC_SCM_RESOURCE_OBSERVATION o;char line[96];DWORD mode,e;unsigned mask;
 memset(&o,0,sizeof(o));
 if(!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode)||
    !GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),&mode))return 2;
 puts("ONE_PROSPECTIVE_RESOURCE_SAMPLE; DIFFERENT_PROCESS; HISTORICAL_CAUSE_UNKNOWN; NOT_FCA_ADMISSION; NO_RETRY_NO_EFFECT");
 puts("Type OBSERVE_FCA_INITIAL_RESOURCES_ONCE only after specific owner approval:");
 if(!fgets(line,sizeof(line),stdin)||strcmp(line,"OBSERVE_FCA_INITIAL_RESOURCES_ONCE\n")){
  puts("RESOURCE_OBSERVATION_CANCELLED consumed=0; NO_QUERY_NO_RETRY");return 2;
 }
 /* sink NULL: output only once after collection, no logging during sampling. */
 e=personal_resource_collect(&o,1U,0);
 printf("FCA_RESOURCE_QUERY first=%lu query_stage=%lu attempts=%lu samples=%lu consumed=1; NO_RETRY\n",e,o.error_stage,o.attempts,o.samples);
 if(e||o.samples!=1U){puts("FCA_RESOURCE_VALUES_UNKNOWN; HISTORICAL_CAUSE_UNKNOWN; AUTHORITY_DENY");return 2;}
 mask=(o.first.handles>128U?1U:0U)|(o.first.physical_available<7239172096ULL?2U:0U)|(o.first.commit_available<3221225472ULL?4U:0U);
 printf("FCA_PROSPECTIVE_RESOURCE handles=%lu physical_available=%llu commit_available=%llu begin=%llu end=%llu frequency=%llu below_mask=%u; MASK1_OBSERVER_HANDLES_2_PHYSICAL_4_COMMIT; DIFFERENT_PROCESS_NOT_ADMISSION\n",
 o.first.handles,o.first.physical_available,o.first.commit_available,o.first.begin,o.first.end,o.first.frequency,mask);
 puts("RESOURCE_OBSERVER_COMPLETE; NO_NEW_ORIGINALS_NO_FCA_NO_SCM_NO_KEY_NO_UPDATE_NO_RETRY; HISTORICAL_RETENTION_OS_QUIESCENCE_UNKNOWN");
 return 0;
}