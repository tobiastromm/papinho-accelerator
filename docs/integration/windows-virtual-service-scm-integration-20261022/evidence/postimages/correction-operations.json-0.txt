#include "server_service_data.h"
#include <string.h>
int ps_parse(int argc,const char *const *argv,PS_REQUEST *out)
{
 int i;const char *c;
 if(!out)return 0;memset(out,0,sizeof(*out));
 if(argc<2||!argv||!argv[1]||strcmp(argv[1],"service"))return -1;
 for(i=0;i<argc;i++)if(!argv[i])return 0;
 if(argc<3)return 0;c=argv[2];
 if(!strcmp(c,"host"))return -1;
 if(!strcmp(c,"install")){
  if(argc!=3)return 0;
  memcpy(out->account,"NT SERVICE\\PapinhoAccelerator",sizeof("NT SERVICE\\PapinhoAccelerator"));out->action=PS_INSTALL;return 1;
 }
 if(!strcmp(c,"startup")){
  if(argc!=4)return 0;
  if(!strcmp(argv[3],"automatic"))out->action=PS_AUTOMATIC;
  else if(!strcmp(argv[3],"manual"))out->action=PS_MANUAL;
  return out->action!=0U;
 }
 if(argc!=3)return 0;
 if(!strcmp(c,"start"))out->action=PS_START;
 else if(!strcmp(c,"stop"))out->action=PS_STOP;
 else if(!strcmp(c,"status"))out->action=PS_STATUS;
 else if(!strcmp(c,"remove"))out->action=PS_REMOVE;
 return out->action!=0U;
}
int ps_begin(PS_LIFETIME *s)
{if(!s||s->phase||s->stop||s->terminal)return 0;s->phase=PS_START_PENDING;return 1;}
int ps_running(PS_LIFETIME *s)
{if(!s||s->phase!=PS_START_PENDING||s->stop||s->terminal)return 0;s->phase=PS_RUNNING;return 1;}
int ps_stop(PS_LIFETIME *s)
{
 if(!s||s->terminal||s->phase==PS_EMPTY||s->phase==PS_RETAINED)return 0;
 s->stop=1U;s->phase=PS_STOP_PENDING;return 1;
}
int ps_finish(PS_LIFETIME *s,int quiescent,int retained)
{
 if(!s||s->terminal||s->phase==PS_EMPTY||s->phase==PS_RETAINED)return 0;
 if(retained){s->stop=1U;s->phase=PS_RETAINED;return 0;}
 if(!quiescent)return 0;
 s->phase=PS_STOPPED;s->terminal=1U;return 1;
}
int ps_action_allowed(unsigned a,int exists,unsigned state)
{
 if(a==PS_STATUS)return 1;
 if(a==PS_INSTALL)return !exists;
 if(!exists)return 0;
 if(a==PS_START)return state==PS_STOPPED;
 if(a==PS_STOP)return state==PS_RUNNING;
 if(a==PS_REMOVE)return state==PS_STOPPED;
 return (a==PS_AUTOMATIC||a==PS_MANUAL)&&(state==PS_STOPPED||state==PS_RUNNING);
}
