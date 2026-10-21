#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "server_application_lifetime_win32.h"
/* Never reused: stale callbacks cannot reach another invocation or sink. */
static struct {
 volatile LONG gate,state;
 const void *network,*config;
 const PAPACC_LOGGER *identity;
 PAPACC_LOGGER sink,owned;
 DWORD owner;
} application;
static void application_log(void *context,const PAPACC_LOG_RECORD *record)
{
 (void)context;
 if(InterlockedCompareExchange(&application.gate,1,0)!=0)return;
 if((application.state==PAPACC_APPLICATION_ACTIVE||application.state==PAPACC_APPLICATION_RETAINED)&&
 application.sink.sink) application.sink.sink(application.sink.sink_context,record);
 InterlockedExchange(&application.gate,0);
}
PAPACC_RESULT papacc_application_parent_register(const void *network,const void *config,const PAPACC_LOGGER *logger)
{
 if(!network||!config||!logger)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(InterlockedCompareExchange(&application.gate,1,0)!=0)return PAPACC_RESULT_INVALID_STATE;
 if(application.state!=PAPACC_APPLICATION_EMPTY){InterlockedExchange(&application.gate,0);return PAPACC_RESULT_INVALID_STATE;}
 application.network=network;application.config=config;application.identity=logger;
 application.sink=*logger;application.owned.sink=application_log;
 application.owned.sink_context=NULL;application.owned.minimum_level=logger->minimum_level;
 application.owner=GetCurrentThreadId();InterlockedExchange(&application.state,PAPACC_APPLICATION_ACTIVE);
 InterlockedExchange(&application.gate,0);return PAPACC_RESULT_OK;
}
PAPACC_BOOL papacc_application_parent_matches(const void *network,const void *config,const PAPACC_LOGGER *logger)
{
 return application.state==PAPACC_APPLICATION_ACTIVE&&application.owner==GetCurrentThreadId()&&
 application.network==network&&application.config==config&&application.identity==logger?PAPACC_TRUE:PAPACC_FALSE;
}
const PAPACC_LOGGER *papacc_application_owned_logger(void){return &application.owned;}
PAPACC_RESULT papacc_application_parent_retain(void)
{
 if(application.owner!=GetCurrentThreadId()||application.state!=PAPACC_APPLICATION_ACTIVE)return PAPACC_RESULT_INVALID_STATE;
 InterlockedExchange(&application.state,PAPACC_APPLICATION_RETAINED);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_application_logger_seal(void)
{
 if(application.owner!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 if(InterlockedCompareExchange(&application.gate,1,0)!=0)return PAPACC_RESULT_INVALID_STATE;
 if(application.state!=PAPACC_APPLICATION_ACTIVE){InterlockedExchange(&application.gate,0);return PAPACC_RESULT_INVALID_STATE;}
 application.sink.sink=NULL;application.sink.sink_context=NULL;
 InterlockedExchange(&application.state,PAPACC_APPLICATION_SEALED);
 InterlockedExchange(&application.gate,0);return PAPACC_RESULT_OK;
}
PAPACC_APPLICATION_LIFETIME_STATE papacc_application_lifetime_state(void)
{return (PAPACC_APPLICATION_LIFETIME_STATE)InterlockedCompareExchange(&application.state,0,0);}
PAPACC_SIZE papacc_application_lifetime_storage_size(void){return sizeof(application);}

