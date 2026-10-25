#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "n0-protected-root-core.h"
#include "n0-protected-root-descriptor.h"
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}++checks;}while(0)
typedef struct FIXTURE {int fail_step,fail_component,stop_at;unsigned calls,creates,closes,retained;DWORD failure,close_failure;} FIXTURE;
static int stop(void *p){FIXTURE *f=(FIXTURE *)p;return f->stop_at>=0&&f->calls>=(unsigned)f->stop_at;}
static DWORD step(FIXTURE *f,unsigned i,int j){++f->calls;if(f->fail_step==j&&f->fail_component==(int)i){if(j==5)f->retained=1;return f->failure;}if(j==2)++f->creates;return 0;}
static DWORD parent(void *p,unsigned i){return step((FIXTURE *)p,i,0);}
static DWORD absent(void *p,unsigned i){return step((FIXTURE *)p,i,1);}
static DWORD create(void *p,unsigned i){return step((FIXTURE *)p,i,2);}
static DWORD open_created(void *p,unsigned i){return step((FIXTURE *)p,i,3);}
static DWORD verify(void *p,unsigned i){return step((FIXTURE *)p,i,4);}
static DWORD finish(void *p,unsigned i){return step((FIXTURE *)p,i,5);}
static DWORD close_all(void *p){FIXTURE *f=(FIXTURE *)p;++f->closes;return f->retained?ERROR_INVALID_STATE:f->close_failure;}
static void init(FIXTURE *f,N0_ROOT_FLOW *s,N0_ROOT_OPS *o)
{
 ZeroMemory(f,sizeof(*f));ZeroMemory(s,sizeof(*s));f->fail_step=-1;f->fail_component=-1;f->stop_at=-1;f->failure=ERROR_ACCESS_DENIED;
 o->context=f;o->stop=stop;o->parent=parent;o->absent=absent;o->create=create;o->open=open_created;o->verify=verify;o->finish=finish;o->close=close_all;
}
/* Historical native parser layout: NEGATIVE exact-byte fixture only. */
static const BYTE alternative_layout[104]={
 0x01,0x00,0x04,0x90,0x48,0x00,0x00,0x00,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x14,0x00,0x00,0x00,0x02,0x00,0x34,0x00,0x02,0x00,0x00,0x00,0x00,0x03,0x14,0x00,
 0xff,0x01,0x1f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,
 0x00,0x03,0x18,0x00,0xff,0x01,0x1f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
 0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
 0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
 0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00
};
int main(void)
{
 unsigned checks=0,i,j,used;BYTE copy[104],aces[44];FIXTURE f;N0_ROOT_FLOW s;N0_ROOT_OPS o;
 T(n0_sd_input(n0_root_descriptor,sizeof(n0_root_descriptor)));
 T(!n0_sd_input(n0_root_descriptor,103));
 T(n0_sd_policy((PSECURITY_DESCRIPTOR)alternative_layout,104));T(!n0_sd_input(alternative_layout,104));
 memcpy(copy,n0_root_descriptor,104);copy[3]^=4;T(!n0_sd_input(copy,104));T(!n0_sd_policy(copy,104));
 memcpy(copy,n0_root_descriptor,104);copy[32]=0x21;T(!n0_sd_policy(copy,104));
 memcpy(copy,n0_root_descriptor,104);copy[48]=0x21;T(!n0_sd_policy(copy,104));
 memcpy(copy,n0_root_descriptor,104);copy[100]=0x21;T(!n0_sd_policy(copy,104));
 memcpy(copy,n0_root_descriptor,104);copy[61]=0x13;T(!n0_sd_policy(copy,104));
 memcpy(copy,n0_root_descriptor,104);copy[84]=0xfe;T(!n0_sd_policy(copy,104));
 memcpy(copy,n0_root_descriptor,104);memcpy(aces,copy+60,44);memcpy(copy+60,aces+20,24);memcpy(copy+84,aces,20);T(!n0_sd_policy(copy,104));
 init(&f,&s,&o);T(!n0_root_run(&s,&o));T(s.created==7&&s.verified==7&&s.finished==7&&f.creates==3&&f.closes==1);
 used=f.calls;T(n0_root_run(&s,&o)==ERROR_INVALID_STATE);T(f.calls==used&&f.closes==1);
 for(i=0;i<3;i++)for(j=0;j<6;j++){
  init(&f,&s,&o);f.fail_component=(int)i;f.fail_step=(int)j;
  T(n0_root_run(&s,&o)==ERROR_ACCESS_DENIED);
  T(f.creates==i+(j>2?1U:0U)&&f.closes==1);
  if(j==5)T(f.retained&&s.close_error==ERROR_INVALID_STATE&&s.first==ERROR_ACCESS_DENIED);
 }
 init(&f,&s,&o);f.fail_step=2;f.fail_component=0;f.failure=ERROR_ALREADY_EXISTS;T(n0_root_run(&s,&o)==ERROR_ALREADY_EXISTS&&f.creates==0);
 init(&f,&s,&o);f.fail_step=4;f.fail_component=1;f.failure=ERROR_REPARSE_TAG_INVALID;T(n0_root_run(&s,&o)==ERROR_REPARSE_TAG_INVALID&&f.creates==2&&s.finished==1);
 init(&f,&s,&o);f.fail_step=0;f.fail_component=0;f.failure=ERROR_INVALID_DATA;T(n0_root_run(&s,&o)==ERROR_INVALID_DATA&&f.creates==0);
 for(i=0;i<18;i++){init(&f,&s,&o);f.stop_at=(int)i;T(n0_root_run(&s,&o)==ERROR_CANCELLED&&f.closes==1&&f.calls==i);}
 init(&f,&s,&o);f.close_failure=ERROR_INVALID_HANDLE;T(n0_root_run(&s,&o)==ERROR_INVALID_HANDLE&&s.created==7&&s.close_error==ERROR_INVALID_HANDLE);
 init(&f,&s,&o);f.fail_step=3;f.fail_component=0;f.failure=ERROR_INVALID_DATA;f.close_failure=ERROR_INVALID_HANDLE;T(n0_root_run(&s,&o)==ERROR_INVALID_DATA&&s.close_error==ERROR_INVALID_HANDLE);
 printf("N0_ROOT_MODEL_PASS checks=%u; ORIGINAL104_HASH_POLICY_ORDER; COLLISION_REPARSE_IDENTITY_PARTIAL_REPLAY_STOP_RETAINED_CLOSE_FIRST_ERROR; VIRTUAL_FIXTURES_ONLY_NO_NATIVE_CREATION\n",checks);
 return 0;
}
