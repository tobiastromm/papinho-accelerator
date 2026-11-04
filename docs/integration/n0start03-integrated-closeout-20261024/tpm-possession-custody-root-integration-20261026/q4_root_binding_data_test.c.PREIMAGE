#define PAPACC_C8_PRIVATE_STORAGE_PROFILE 1
#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT root;
static PAPACC_CONFIG_PIN pin;
static unsigned checks;
static int owner(PAPACC_CONFIGURATION_COMMIT *o){return o&&o->initialized&&o->writer==GetCurrentThreadId();}
static int pending_blocks(PAPACC_CONFIGURATION_COMMIT *o){return o->pending_commit.active||o->authority_lost;}
static int issuer06_overlap(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE m){ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return x<=y?y-x<n:x-y<m;}
#include "server_configuration_q4_data_win32.inc"
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static void reset(void){root.published=1;root.initialized=1;root.writer=GetCurrentThreadId();root.domain[0]=1;root.current=0;root.slots[0].view.domain[0]=1;root.slots[0].view.revision=2;root.slots[0].view.predecessor=1;root.slots[0].context_epoch=3;root.slots[0].operational_epoch=4;root.slots[0].readers=1;root.slots[0].serial=5;pin.issuer=&root;pin.index=0;pin.serial=5;pin.ticket=6;root.tickets[0]=pin;}
static DWORD WINAPI foreign(LPVOID p){PAPACC_Q4_ROOT_BINDING_DATA out;(void)p;return papacc_configuration_q4_binding_data(&root,&pin,&out)==PAPACC_RESULT_INVALID_ARGUMENT?0:1;}
int main(void){PAPACC_Q4_ROOT_BINDING_DATA out,saved;HANDLE t;DWORD e;unsigned i;
 memset(&root,0,sizeof(root));InitializeCriticalSection(&root.lock);reset();memset(&out,0xa5,sizeof(out));saved=out;
 C(papacc_configuration_q4_binding_data(&root,&pin,&out)==PAPACC_RESULT_OK);C(out.version==1&&out.scope_data_only==1&&out.ticket==6&&out.policy_epoch==4);C(root.slots[0].readers==1&&root.tickets[0].ticket==6);out=saved;
 for(i=0;i<12;i++){reset();switch(i){case 0:root.stopped=1;break;case 1:root.unresolved=1;break;case 2:root.published=0;break;case 3:root.slots[0].retained=1;break;case 4:pin.serial++;break;case 5:pin.ticket++;break;case 6:root.slots[0].view.domain[0]++;break;case 7:root.slots[0].view.predecessor=0;break;case 8:root.slots[0].context_epoch=0;break;case 9:root.slots[0].operational_epoch=0;break;case 10:root.prepared=1;break;case 11:root.pending_commit.active=1;break;}
 C(papacc_configuration_q4_binding_data(&root,&pin,&out)!=PAPACC_RESULT_OK);C(!memcmp(&out,&saved,sizeof(out)));root.stopped=root.unresolved=root.prepared=root.pending_commit.active=0;root.slots[0].retained=0;}
 reset();root.domain[0]=0;root.slots[0].view.domain[0]=0;C(papacc_configuration_q4_binding_data(&root,&pin,&out)!=PAPACC_RESULT_OK);C(!memcmp(&out,&saved,sizeof(out)));
 reset();C(papacc_configuration_q4_binding_data(&root,&pin,(PAPACC_Q4_ROOT_BINDING_DATA*)&root)==PAPACC_RESULT_INVALID_ARGUMENT);C(root.domain[0]==1);C(papacc_configuration_q4_binding_data(&root,&pin,(PAPACC_Q4_ROOT_BINDING_DATA*)&pin)==PAPACC_RESULT_INVALID_ARGUMENT);C(pin.ticket==6);
 t=CreateThread(NULL,0,foreign,NULL,0,NULL);C(t!=NULL);C(WaitForSingleObject(t,5000)==WAIT_OBJECT_0);C(GetExitCodeThread(t,&e)&&e==0);C(CloseHandle(t));
 root.initialized=0;DeleteCriticalSection(&root.lock);printf("PASS checks=%u binding_bytes=%zu root_bytes=%zu NO_STORE_NO_TPM_NO_AUTHORITY\n",checks,sizeof(out),sizeof(root));return 0;}
