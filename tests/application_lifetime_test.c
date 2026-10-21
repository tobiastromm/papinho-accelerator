#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "server_application_lifetime_win32.h"
#include <stdio.h>
#include <string.h>
static int network,config,wrong,delivered;
static PAPACC_LOGGER parent;
static PAPACC_RESULT seal_during_callback;
static void sink(void *context,const PAPACC_LOG_RECORD *record)
{
 (void)record;if(context!=&delivered)return;
 ++delivered;seal_during_callback=papacc_application_logger_seal();
}
static const PAPACC_LOGGER *prepare_in_frame(void)
{
 parent.sink=sink;parent.sink_context=&delivered;parent.minimum_level=PAPACC_LOG_INFO;
 if(papacc_application_parent_register(&network,&config,&parent)!=PAPACC_RESULT_OK)return NULL;
 return papacc_application_owned_logger();
}
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(int argc,char **argv)
{
 unsigned checks=0;const PAPACC_LOGGER *logger;PAPACC_LOG_RECORD record;
 int retained=argc>1&&strcmp(argv[1],"retained")==0;
 memset(&record,0,sizeof(record));
#ifdef PAPACC_ENABLE_CANONICAL_API3_CANDIDATE
 CHECK(papacc_application_require_operational_admission()!=PAPACC_RESULT_OK);
#endif
 CHECK(papacc_application_parent_register(NULL,&config,&parent)==PAPACC_RESULT_INVALID_ARGUMENT);
 CHECK(papacc_application_lifetime_state()==PAPACC_APPLICATION_EMPTY);
 logger=prepare_in_frame();CHECK(logger!=NULL);
 CHECK(papacc_application_parent_matches(&network,&config,&parent));
 CHECK(!papacc_application_parent_matches(&wrong,&config,&parent));
#ifdef PAPACC_ENABLE_CANONICAL_API3_CANDIDATE
 CHECK(papacc_application_require_operational_admission()!=PAPACC_RESULT_OK);
#endif
 CHECK(papacc_application_parent_register(&network,&config,&parent)==PAPACC_RESULT_INVALID_STATE);
 /* The creating frame is gone. Parent, sink context and bridge are static. */
 logger->sink(logger->sink_context,&record);
 CHECK(delivered==1&&seal_during_callback==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_application_lifetime_state()==PAPACC_APPLICATION_ACTIVE);
 if(retained){
 CHECK(papacc_application_parent_retain()==PAPACC_RESULT_OK);
 CHECK(papacc_application_logger_seal()==PAPACC_RESULT_INVALID_STATE);
 logger->sink(logger->sink_context,&record);CHECK(delivered==2);
 CHECK(papacc_application_lifetime_state()==PAPACC_APPLICATION_RETAINED);
 }else{
 CHECK(papacc_application_logger_seal()==PAPACC_RESULT_OK);
 CHECK(papacc_application_lifetime_state()==PAPACC_APPLICATION_SEALED);
 logger->sink(logger->sink_context,&record);CHECK(delivered==1);
 CHECK(papacc_application_logger_seal()==PAPACC_RESULT_INVALID_STATE);
 }
#ifdef PAPACC_ENABLE_CANONICAL_API3_CANDIDATE
 CHECK(papacc_application_require_operational_admission()!=PAPACC_RESULT_OK);
#endif
 CHECK(papacc_application_parent_register(&network,&config,&parent)==PAPACC_RESULT_INVALID_STATE);
 printf("PASS checks=%u retained=%d delivered=%d\n",checks,retained,delivered);return 0;
}