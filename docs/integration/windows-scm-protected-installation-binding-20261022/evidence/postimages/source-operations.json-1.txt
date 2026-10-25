#include "server_service_data.h"
#include <string.h>
int ps_parse(int argc,const char *const *argv,PS_REQUEST *out)
{
 int i;const char *c;size_t n;
 if(!out)return 0;memset(out,0,sizeof(*out));
 if(argc<2||!argv||!argv[1]||strcmp(argv[1],"service"))return -1;
 for(i=0;i<argc;i++)if(!argv[i])return 0;
 if(argc<3)return 0;c=argv[2];
 if(!strcmp(c,"host"))return -1;
 if(!strcmp(c,"install")){
  if(argc!=3){
   if(argc!=8||strcmp(argv[3],"--prepare-lab")||strcmp(argv[4],"--installation")||strcmp(argv[6],"--configuration"))return 0;
   n=strlen(argv[5]);if(!n||n>=sizeof(out->installation))return 0;memcpy(out->installation,argv[5],n+1);
   n=strlen(argv[7]);if(!n||n>=sizeof(out->configuration)||!strcmp(argv[7],".")||!strcmp(argv[7],".."))return 0;
   for(i=0;i<(int)n;i++)if(!((argv[7][i]>='a'&&argv[7][i]<='z')||(argv[7][i]>='A'&&argv[7][i]<='Z')||
    (argv[7][i]>='0'&&argv[7][i]<='9')||argv[7][i]=='_'||argv[7][i]=='-'||argv[7][i]=='.'))return 0;
   memcpy(out->configuration,argv[7],n+1);out->prepare_lab=1;
  }
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

static int ps_nonzero(const unsigned char *p,unsigned n)
{unsigned i;unsigned char v=0;for(i=0;i<n;i++)v=(unsigned char)(v|p[i]);return v!=0;}
int ps_lab_binding_capture(PS_LAB_BINDING *b,const PS_BINDING_FACTS *f)
{
 if(!b||!f||b->phase||!f->configuration_bytes||f->configuration_bytes>8192U||
 !(f->installation[1]|f->installation[2])||!(f->configuration[1]|f->configuration[2])||!(f->executable[1]|f->executable[2])||
 !ps_nonzero(f->attempt,32)||!ps_nonzero(f->operator_id,32)||!ps_nonzero(f->config_hash,32)||!ps_nonzero(f->image_hash,32)||
 !ps_nonzero(f->parent_acl,32)||!ps_nonzero(f->config_acl,32)||!ps_nonzero(f->image_acl,32))return 0;
 b->facts=*f;b->phase=1U;return 1;
}
int ps_lab_binding_complete(PS_LAB_BINDING *b,const PS_BINDING_FACTS *f,int stopped)
{
 if(!b||!f||b->phase!=1U)return 0;
 if(stopped||memcmp(&b->facts,f,sizeof(*f))){b->phase=3U;return 0;}
 b->phase=2U;return 1; /* read-only preparation consumed, not effect authority */
}
void ps_lab_binding_invalidate(PS_LAB_BINDING *b){if(b)b->phase=3U;}
