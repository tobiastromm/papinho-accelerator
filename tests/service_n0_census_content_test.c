/* Exact product scope/wrapper; callbacks simulated, no admission. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../tools/qualification/scm-n0-census-content.h"
#define PAPACC_N0_N9_NATIVE_OWNER 1
typedef struct {unsigned n0;ULONGLONG challenge;} DESC;
static struct {struct {struct {unsigned bound;DESC binding;void *check;} bd;} admission;} native;
static struct {ULONGLONG began,validated_at;unsigned metric_window;} ig_n0;
static IG_N0_CENSUS_CONTENT *ig_n0_census_content;
static unsigned ig_n0_positive=1,entries,mode,calls,hashes,checks,tests,stop,closed;
static ULONGLONG clock_now;
static DWORD last_error;
static DWORD lab_aux_gate(void){return stop?ERROR_OPERATION_ABORTED:0;}
static DWORD ig_project_census(void);
static DWORD current(void){DWORD e;calls++;if((e=lab_aux_gate()))return e;
 if(clock_now<ig_n0.began||clock_now-ig_n0.began>60000)return ERROR_TIMEOUT;
 /* Volatile failure that would disappear before the final boundary. */
 if(mode>=1&&mode<=6&&calls==entries/2)return ERROR_ACCESS_DENIED;
 if(ig_n0_census_content){
  if(!ig_n0_census_content_valid(ig_n0_census_content,&native.admission.bd.binding,ig_n0.began,ig_n0.validated_at,native.admission.bd.binding.challenge,GetCurrentThreadId(),clock_now))return ERROR_TIMEOUT;
  ig_n0_census_content->calls++;
 }else{hashes++;if(mode==7&&hashes==1)return ERROR_READ_FAULT;
  if(mode==8&&hashes==2)return ERROR_CRC;
  ig_n0.validated_at=clock_now;}
 return 0;}
static DWORD papacc_server_service_admission_bd_revalidate(void *check){(void)check;checks++;return current();}
static DWORD ig_project_census_body(void){unsigned i;DWORD e;
 for(i=0;i<entries;i++){
  if(mode==9&&i==entries/2)clock_now+=60001;
  if(mode==10&&i==entries/2)stop=1;
  if(mode==11&&i==entries/2){e=ig_project_census();if(e)return e;}
  if(mode==12&&i==entries/2)ig_n0.validated_at++;
  if(mode==13&&i==entries/2)native.admission.bd.binding.challenge++;
  if(mode==14&&i==entries/2)return ERROR_INVALID_HANDLE;
  if((e=current()))return e;
 }
 if(mode==15)clock_now+=60001;
 closed=1;return 0;}
static ULONGLONG fixture_tick(void){return clock_now;}
#define GetTickCount64 fixture_tick
#include "../tools/qualification/scm-n0-census-content.inc"
#undef GetTickCount64
#define T(x) do{tests++;if(!(x)){printf("FAIL line=%d mode=%u error=%lu\n",__LINE__,mode,(unsigned long)last_error);return 1;}}while(0)
int main(void){unsigned m,n;IG_N0_CENSUS_CONTENT s;DESC d;ULONGLONG began;
 ZeroMemory(&d,sizeof(d));s.descriptor=&d;s.began=100;s.validated=120;s.challenge=4;s.thread=GetCurrentThreadId();s.calls=0;
 T(ig_n0_census_content_valid(&s,&d,100,120,4,s.thread,60100));
 T(!ig_n0_census_content_valid(&s,&d,100,120,4,s.thread,60101));
 T(!ig_n0_census_content_valid(&s,&d,100,120,4,s.thread,119));
 T(!ig_n0_census_content_valid(&s,&s,100,120,4,s.thread,120));
 T(!ig_n0_census_content_valid(&s,&d,100,121,4,s.thread,121));
 T(!ig_n0_census_content_valid(&s,&d,100,120,5,s.thread,120));
 T(!ig_n0_census_content_valid(&s,&d,100,120,4,s.thread+1,120));
 for(n=32;n<=1024;n*=2)for(m=0;m<=16;m++){
  mode=m;entries=n;calls=hashes=checks=stop=closed=0;clock_now=1000;
  ig_n0.began=100;ig_n0.validated_at=0;ig_n0.metric_window=0;
  native.admission.bd.bound=1;native.admission.bd.binding.n0=1;native.admission.bd.binding.challenge=4;
  if(m==16)clock_now=60101;
  began=ig_n0.began;last_error=ig_project_census();
  T(ig_n0_census_content==NULL);T(ig_n0.began==began);
  if(!m){T(last_error==0);T(hashes==2);T(calls==n+2);T(closed);}
  else{T(last_error!=0);if(m==11)T(last_error==ERROR_INVALID_STATE);}
  T(checks<=2);
 }
 mode=0;stop=0;clock_now=1000;ig_n0.began=100;entries=4;calls=hashes=checks=0;ig_n0_positive=0;
 T(ig_project_census()==0);T(hashes==4&&checks==0);
 printf("CENSUS_CONTENT=PASS assertions=%u scope_bytes=%zu pointer_bytes=%zu; EXACT_WRAPPER_SIMULATED_CALLBACKS_NO_LAB\n",tests,sizeof(s),sizeof(ig_n0_census_content));return 0;}
