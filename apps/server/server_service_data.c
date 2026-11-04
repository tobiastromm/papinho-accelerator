#include "server_service_data.h"
#include <string.h>
static int ps_carrier_nonzero(const unsigned char *v,unsigned n)
{unsigned i;unsigned char b=0;for(i=0;i<n;i++)b=(unsigned char)(b|v[i]);return b!=0;}
static int ps_carrier_valid(const PS_SCQ_CARRIER *p)
{
 unsigned i;
 if(!p||!p->controller_pid||!(p->controller_creation[0]|p->controller_creation[1])||!(p->until[0]|p->until[1])||
  p->runtime_sid_bytes<8U||p->runtime_sid_bytes>68U||p->runtime_sid[0]!=1U||p->runtime_sid[1]>15U||p->runtime_sid_bytes!=8U+4U*p->runtime_sid[1])return 0;
 for(i=p->runtime_sid_bytes;i<68U;i++)if(p->runtime_sid[i])return 0;
 return ps_carrier_nonzero(p->attempt,32)&&ps_carrier_nonzero(p->policy,32)&&ps_carrier_nonzero(p->target,32)&&
  ps_carrier_nonzero(p->request,32)&&ps_carrier_nonzero(p->decision,32)&&ps_carrier_nonzero(p->image,32)&&
  ps_carrier_nonzero(p->endpoint,16);
}
static void ps_carrier_put(unsigned char *b,unsigned v)
{unsigned i;for(i=0;i<4U;i++)b[i]=(unsigned char)(v>>(8U*i));}
static unsigned ps_carrier_get(const unsigned char *b)
{unsigned i,v=0;for(i=0;i<4U;i++)v|=(unsigned)b[i]<<(8U*i);return v;}
static void ps_carrier_pack(const PS_SCQ_CARRIER *p,unsigned char b[PS_SCQ_CARRIER_BYTES])
{
 unsigned w[8],i;w[0]=1U;w[1]=2U;w[2]=p->controller_pid;w[3]=p->controller_creation[0];w[4]=p->controller_creation[1];w[5]=p->until[0];w[6]=p->until[1];w[7]=p->runtime_sid_bytes;
 for(i=0;i<8U;i++)ps_carrier_put(b+4U*i,w[i]);
 memcpy(b+32,p->attempt,32);memcpy(b+64,p->policy,32);memcpy(b+96,p->target,32);memcpy(b+128,p->request,32);memcpy(b+160,p->decision,32);memcpy(b+192,p->image,32);
 memcpy(b+224,p->endpoint,16);memcpy(b+240,p->runtime_sid,68);
}
int ps_scq_carrier_encode(const PS_SCQ_CARRIER *p,char *out,unsigned cap)
{
 static const char hex[]="0123456789abcdef";unsigned char b[PS_SCQ_CARRIER_BYTES];unsigned i;
 if(!out||cap<PS_SCQ_CARRIER_CHARS||!ps_carrier_valid(p))return 0;
 ps_carrier_pack(p,b);memcpy(out,"SCQ1:",5);
 for(i=0;i<PS_SCQ_CARRIER_BYTES;i++){out[5U+2U*i]=hex[b[i]>>4];out[6U+2U*i]=hex[b[i]&15U];}out[PS_SCQ_CARRIER_CHARS-1U]=0;return 1;
}
static int ps_carrier_hex(char c)
{if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;return -1;}
int ps_scq_carrier_decode(const char *s,unsigned bytes,PS_SCQ_CARRIER *out)
{
 PS_SCQ_CARRIER p;unsigned char b[PS_SCQ_CARRIER_BYTES];unsigned i;int a,c;
 if(!out||!s||bytes!=PS_SCQ_CARRIER_CHARS-1U||memcmp(s,"SCQ1:",5))return 0;
 for(i=0;i<PS_SCQ_CARRIER_BYTES;i++){a=ps_carrier_hex(s[5U+2U*i]);c=ps_carrier_hex(s[6U+2U*i]);if(a<0||c<0)return 0;b[i]=(unsigned char)(16*a+c);}
 if(ps_carrier_get(b)!=1U||ps_carrier_get(b+4)!=2U)return 0;
 memset(&p,0,sizeof(p));p.controller_pid=ps_carrier_get(b+8);p.controller_creation[0]=ps_carrier_get(b+12);p.controller_creation[1]=ps_carrier_get(b+16);p.until[0]=ps_carrier_get(b+20);p.until[1]=ps_carrier_get(b+24);p.runtime_sid_bytes=ps_carrier_get(b+28);
 memcpy(p.attempt,b+32,32);memcpy(p.policy,b+64,32);memcpy(p.target,b+96,32);memcpy(p.request,b+128,32);memcpy(p.decision,b+160,32);memcpy(p.image,b+192,32);memcpy(p.endpoint,b+224,16);memcpy(p.runtime_sid,b+240,68);
 if(!ps_carrier_valid(&p))return 0;*out=p;return 1;
}
int ps_scq_carrier_equal(const PS_SCQ_CARRIER *a,const PS_SCQ_CARRIER *b)
{unsigned char x[PS_SCQ_CARRIER_BYTES],y[PS_SCQ_CARRIER_BYTES];if(!ps_carrier_valid(a)||!ps_carrier_valid(b))return 0;ps_carrier_pack(a,x);ps_carrier_pack(b,y);return !memcmp(x,y,sizeof(x));}
int ps_scq_remaining_ms(const PS_SCQ_CARRIER *p,unsigned now_low,unsigned now_high,unsigned *out)
{
 unsigned long long original,global,now,until,remaining;
 if(!p||!out)return 0;
 original=((unsigned long long)p->until[1]<<32)|p->until[0];global=((unsigned long long)p->global_until[1]<<32)|p->global_until[0];
 now=((unsigned long long)now_high<<32)|now_low;if(!original||!global)return 0;
 until=original<global?original:global;if(until<=now)return 0;remaining=until-now;
 if(remaining>60000U)return 0;*out=(unsigned)remaining;return 1;
}
int ps_parse(int argc,const char *const *argv,PS_REQUEST *out)
{
 int i;const char *c;size_t n;
 if(!out)return 0;memset(out,0,sizeof(*out));
 if(argc<2||!argv||!argv[1]||strcmp(argv[1],"service"))return -1;
 for(i=0;i<argc;i++)if(!argv[i])return 0;
 if(argc<3)return 0;c=argv[2];
 if(!strcmp(c,"host"))return -1;
 if(!strcmp(c,"install")){
  if(argc==4&&!strcmp(argv[3],"--scq-closed"))out->install_profile=PS_INSTALL_SCQ_CLOSED;
  else if(argc!=3){
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
 if(!strcmp(c,"start")&&argc==4&&!strcmp(argv[3],"--scq-closed")){out->action=PS_START;out->start_profile=PS_START_SCQ_CLOSED;return 1;}
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
