#include "storage20_delivery.h"
#include <stdio.h>
#include <string.h>
static TP_OWNER o,unknown_owner[3];
static ST20_DELIVERY d,unknown_delivery[3];
static ST20_API api;
static unsigned char *scratch;static unsigned checks,gets,uses,guards,mode,guard_mode,unlock_mode,lock_mode,clear_mode,clear_calls;
static HANDLE start_event;
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u check=%u\n",(unsigned)__LINE__,checks);return 2;}}while(0)
static int zeros(const unsigned char *p,unsigned n){unsigned i,v=0;for(i=0;i<n;i++)v|=p[i];return v==0;}
static TBS_RESULT WINAPI get_mock(TBS_HCONTEXT context,TBS_OWNERAUTH_TYPE type,PBYTE output,PUINT32 length){unsigned i;gets++;if(context!=(void *)1||type!=13||!output||*length!=64)return 0x80284002UL;
 if(mode==1)return 0x80284015UL;for(i=0;i<32;i++)output[i]=(unsigned char)(i+1U);
 if(mode==2)return 0x80284001UL;if(mode==3){*length=0;return 0;}if(mode==4){*length=65;return 0;}if(mode==5)output[0]^=1;*length=32;return 0;}
static BOOL WINAPI lock_buffer(LPVOID p,SIZE_T n){if(lock_mode){SetLastError(ERROR_WORKING_SET_QUOTA);return FALSE;}return VirtualLock(p,n);}
static BOOL WINAPI unlock_buffer(LPVOID p,SIZE_T n){if(unlock_mode){SetLastError(ERROR_LOCK_FAILED);return FALSE;}return VirtualUnlock(p,n);}
static BOOL clear_buffer(unsigned char *p,unsigned n){clear_calls++;if(clear_mode&&clear_calls==2){SetLastError(ERROR_WRITE_FAULT);return FALSE;}return st20_secure_clear(p,n);}
static int check_guard(TP_OWNER *owner,const ST20_DELIVERY *delivery,const char *phase){(void)delivery;guards++;
 if(guard_mode==4){owner->context=(void *)2;return 0;}
 if(guard_mode==1&&strcmp(phase,"BEFORE_GET")==0)return 0; /* wrong designated custodian/source */
 if(guard_mode==2&&strcmp(phase,"BEFORE_USE")==0)owner->context=(void *)2;
 if(guard_mode==3&&strcmp(phase,"AFTER_USE")==0)owner->stop=1;
 return 1;}
static int consume(TP_OWNER *owner,const unsigned char *p,unsigned n){unsigned i;uses++;if(mode==6){(void)st20_deliver(&d,&api);return 1;}if(mode==7)return 2;
 if(owner->context!=(void *)1||n!=32)return 0;for(i=0;i<32;i++)if(p[i]!=(unsigned char)(i+1U))return 0;return 1;}
static void reset(void){tp_init(&o);o.context=(void *)1;memset(&d,0,sizeof(d));d.owner=&o;d.original_context=(void *)1;d.buffer=scratch;d.capacity=64;memset(scratch,0xa5,64);gets=uses=guards=mode=guard_mode=unlock_mode=lock_mode=clear_mode=clear_calls=0;api.get=get_mock;api.lock=lock_buffer;api.unlock=unlock_buffer;api.guard=check_guard;api.use=consume;api.clear=clear_buffer;}
static DWORD WINAPI foreign(LPVOID unused){(void)unused;if(WaitForSingleObject(start_event,5000)!=WAIT_OBJECT_0)return 99;return st20_deliver(&d,&api)?98:0;}
int main(void){unsigned i,oldgets;unsigned char *original;HANDLE threads[2];DWORD code;const char *first;
 scratch=(unsigned char *)VirtualAlloc(NULL,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);C(scratch!=NULL);
 reset();C(st20_deliver(&d,&api));C(gets==1&&uses==1&&d.wiped&&!d.locked&&!d.retained&&!o.retained&&zeros(scratch,64));C(d.length==0&&o.busy==0);
 oldgets=gets;C(!st20_deliver(&d,&api));C(gets==oldgets&&o.frozen);
 for(i=1;i<=5;i++){reset();mode=i;C(!st20_deliver(&d,&api));C(gets==1&&o.frozen&&d.wiped&&!d.locked&&zeros(scratch,64));C(uses==(i==5?1U:0U));C(!st20_deliver(&d,&api)&&gets==1);if(i==1)C(o.first_native==0x80284015UL);}
 reset();lock_mode=1;C(!st20_deliver(&d,&api));C(gets==0&&!d.locked&&o.frozen);
 reset();guard_mode=1;C(!st20_deliver(&d,&api));C(gets==0&&uses==0&&d.wiped&&zeros(scratch,64));
 reset();o.context=(void *)2;C(!st20_deliver(&d,&api));C(gets==0&&o.retained&&d.retained);
 reset();guard_mode=2;C(!st20_deliver(&d,&api));C(gets==1&&uses==0&&o.retained&&d.retained&&d.wiped&&zeros(scratch,64));
 reset();guard_mode=4;C(!st20_deliver(&d,&api));C(gets==0&&o.retained&&d.retained);
 reset();guard_mode=3;C(!st20_deliver(&d,&api));C(gets==1&&uses==1&&d.wiped&&zeros(scratch,64));
 reset();mode=6;C(!st20_deliver(&d,&api));C(gets==1&&uses==1&&o.frozen&&d.wiped&&zeros(scratch,64));
 reset();original=d.buffer;d.buffer=(unsigned char *)&o;C(!st20_deliver(&d,&api)&&gets==0);d.buffer=original;
 reset();d.buffer=(unsigned char *)&d;C(!st20_deliver(&d,&api)&&gets==0);
 reset();d.buffer=(unsigned char *)&api;C(!st20_deliver(&d,&api)&&gets==0);
 reset();d.capacity=65;C(!st20_deliver(&d,&api)&&gets==0);
 reset();d.capacity=0;C(!st20_deliver(&d,&api)&&gets==0);
 reset();o.stop=1;C(!st20_deliver(&d,&api)&&gets==0);
 reset();start_event=CreateEventW(NULL,TRUE,FALSE,NULL);C(start_event!=NULL);
 for(i=0;i<2;i++){threads[i]=CreateThread(NULL,0,foreign,NULL,0,NULL);C(threads[i]!=NULL);}C(SetEvent(start_event));
 for(i=0;i<2;i++){C(WaitForSingleObject(threads[i],5000)==WAIT_OBJECT_0);C(GetExitCodeThread(threads[i],&code)&&code==0);C(CloseHandle(threads[i]));}C(CloseHandle(start_event));C(gets==0&&!o.frozen&&d.used==0);C(st20_deliver(&d,&api)&&gets==1);
 C(VirtualFree(scratch,0,MEM_RELEASE));
 /* UNKNOWN cases retain original synthetic buffer/page/context dependencies in
  * static lifetime until fixture process loss, not a claimed operational close. */
 for(i=0;i<3;i++){
 ST20_DELIVERY *u=&unknown_delivery[i];TP_OWNER *parent=&unknown_owner[i];
 scratch=(unsigned char *)VirtualAlloc(NULL,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);C(scratch!=NULL);
 tp_init(parent);parent->context=(void *)1;memset(u,0,sizeof(*u));u->owner=parent;u->original_context=(void *)1;u->buffer=scratch;u->capacity=64;
 mode=i==0?7U:0U;unlock_mode=i==1?1U:0U;guard_mode=lock_mode=gets=uses=guards=clear_calls=0;clear_mode=i==2?1U:0U;
 C(!st20_deliver(u,&api));C(u->retained&&u->locked&&parent->retained&&parent->frozen&&gets==1);C(i==1?u->wiped&&zeros(scratch,64):!u->wiped&&u->length==32);first=parent->first;C(!st20_deliver(u,&api)&&gets==1&&parent->first==first);C(!tp_finish(parent));C(u->locked&&parent->retained&&parent->busy==1&&parent->first==first);
 }

 printf("LAYOUT borrow=%zu api=%zu capacity=%u; NOT_TOTAL_MEMORY\n",sizeof(ST20_DELIVERY),sizeof(ST20_API),ST20_CAPACITY_LIMIT);
 printf("PASS checks=%u; SOURCE_API_MOCK_ONLY; WINDOWS_LOCK_WIPE_THREADS_REAL; REAL_OWNERAUTH_READS=0; NV=NONE; UNKNOWN_PAGES_RETAINED_UNTIL_PROCESS_LOSS; NO_AUTHORITY\n",checks);return 0;
}
