#define PAPACC_C8_PRIVATE_STORAGE_PROFILE 1
#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
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
static PAPACC_Q4_TC_REVIEW tc;
static int tc_hex(unsigned char out[32],const char *s){unsigned i,x;for(i=0;i<32U;i++){if(sscanf_s(s+2U*i,"%2x",&x)!=1)return 0;out[i]=(unsigned char)x;}return 1;}
static DWORD WINAPI tc_foreign(LPVOID p){PAPACC_Q4_TC_POLICY_DATA out;(void)p;return papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_INVALID_ARGUMENT?0:1;}
static int tc_tests(void)
{
 PAPACC_Q4_TC_POLICY_DATA out,saved;unsigned i,j;unsigned char *bytes;HANDLE t;DWORD code;
 reset();memset(&tc,0,sizeof(tc));tc.version=tc.profile=1U;
 C(papacc_configuration_q4_binding_data(&root,&pin,&tc.root)==PAPACC_RESULT_OK);
 C(tc_hex(tc.ek_spki,"1c643066b17bdf9e81c09870661aeb7c9cfff1f2ffbef71b43532c14a25c474e"));
 C(tc_hex(tc.certificate,"965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761"));
 C(tc_hex(tc.anchor,"2e1b3ba79af56d758be51697621bc4b9e8cee0983db3e749c55eb9b37c6d2ae0"));
 memset(tc.approved_generation,1,32);memset(tc.current_generation,1,32);
 memset(tc.approved_custody,2,32);memset(tc.current_custody,2,32);
 memset(tc.original_predecessor,3,32);memset(tc.current_predecessor,3,32);
 for(i=0;i<12U;i++)tc.facts[i]=1U;
 tc.baseline=41;tc.expected=42;tc.observed=41;tc.index=0x01804143U;tc.attributes=0x00040014U;tc.data_bytes=8;
 C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);
 C(out.version==1U&&out.scope_data_only==1U&&out.authority_denied==1U&&out.missing==PAPACC_Q4_TC_SOURCE_MISSING);
 /* Synthetic consistent claims can NEVER manufacture native authority. */
 for(i=0;i<12U;i++)for(j=0;j<4U;j++)if(j!=1U){tc.facts[i]=j;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C((out.missing&(1UL<<i))&&out.authority_denied==1U);tc.facts[i]=1U;}
 bytes=(unsigned char *)&tc+offsetof(PAPACC_Q4_TC_REVIEW,ek_spki);
 for(i=0;i<96U;i++){bytes[i]^=1U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<12));bytes[i]^=1U;}
 for(i=0;i<32U;i++){tc.current_generation[i]^=1U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<13));tc.current_generation[i]^=1U;}
 tc.current_custody[0]^=1U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<14));tc.current_custody[0]^=1U;
 tc.current_predecessor[0]^=1U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<15));tc.current_predecessor[0]^=1U;
 tc.observed=40;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<17));tc.observed=41;
 tc.baseline=(PAPACC_U64)-1;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<17));tc.baseline=41;
 tc.written=1;tc.attributes=0x20040014U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(!(out.missing&(1UL<<16)));tc.attributes=0x40014U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_OK);C(out.missing&(1UL<<16));tc.written=0;
 memset(&out,0xa5,sizeof(out));saved=out;tc.profile=2U;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)==PAPACC_RESULT_INVALID_ARGUMENT);C(!memcmp(&out,&saved,sizeof(out)));tc.profile=1U;
 root.stopped=1;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)!=PAPACC_RESULT_OK);C(!memcmp(&out,&saved,sizeof(out)));root.stopped=0;
 pin.ticket++;C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,&out)!=PAPACC_RESULT_OK);pin.ticket--;
 C(papacc_configuration_q4_tc_policy_data(&root,&pin,&tc,(PAPACC_Q4_TC_POLICY_DATA *)&tc)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_configuration_q4_tc_policy_data(&root,&pin,(const PAPACC_Q4_TC_REVIEW *)&root,&out)==PAPACC_RESULT_INVALID_ARGUMENT);
 for(i=0;i<2U;i++){t=CreateThread(NULL,0,tc_foreign,NULL,0,NULL);C(t!=NULL);C(WaitForSingleObject(t,5000)==WAIT_OBJECT_0);C(GetExitCodeThread(t,&code)&&code==0);C(CloseHandle(t));}
 printf("TC_DATA review_bytes=%zu output_bytes=%zu always_authority_denied=1\n",sizeof(tc),sizeof(out));return 0;
}
int main(void){PAPACC_Q4_ROOT_BINDING_DATA out,saved;PAPACC_Q4_ST20_DESIGNATION_DATA designation;HANDLE t;DWORD e;unsigned i;
 memset(&root,0,sizeof(root));InitializeCriticalSection(&root.lock);reset();memset(&out,0xa5,sizeof(out));saved=out;
 C(papacc_configuration_q4_binding_data(&root,&pin,&out)==PAPACC_RESULT_OK);C(out.version==1&&out.scope_data_only==1&&out.ticket==6&&out.policy_epoch==4);C(root.slots[0].readers==1&&root.tickets[0].ticket==6);out=saved;
 for(i=0;i<12;i++){reset();switch(i){case 0:root.stopped=1;break;case 1:root.unresolved=1;break;case 2:root.published=0;break;case 3:root.slots[0].retained=1;break;case 4:pin.serial++;break;case 5:pin.ticket++;break;case 6:root.slots[0].view.domain[0]++;break;case 7:root.slots[0].view.predecessor=0;break;case 8:root.slots[0].context_epoch=0;break;case 9:root.slots[0].operational_epoch=0;break;case 10:root.prepared=1;break;case 11:root.pending_commit.active=1;break;}
 C(papacc_configuration_q4_binding_data(&root,&pin,&out)!=PAPACC_RESULT_OK);C(!memcmp(&out,&saved,sizeof(out)));root.stopped=root.unresolved=root.prepared=root.pending_commit.active=0;root.slots[0].retained=0;}
 reset();root.domain[0]=0;root.slots[0].view.domain[0]=0;C(papacc_configuration_q4_binding_data(&root,&pin,&out)!=PAPACC_RESULT_OK);C(!memcmp(&out,&saved,sizeof(out)));
 reset();C(papacc_configuration_q4_binding_data(&root,&pin,(PAPACC_Q4_ROOT_BINDING_DATA*)&root)==PAPACC_RESULT_INVALID_ARGUMENT);C(root.domain[0]==1);C(papacc_configuration_q4_binding_data(&root,&pin,(PAPACC_Q4_ROOT_BINDING_DATA*)&pin)==PAPACC_RESULT_INVALID_ARGUMENT);C(pin.ticket==6);
 t=CreateThread(NULL,0,foreign,NULL,0,NULL);C(t!=NULL);C(WaitForSingleObject(t,5000)==WAIT_OBJECT_0);C(GetExitCodeThread(t,&e)&&e==0);C(CloseHandle(t));
 reset();C(papacc_configuration_q4_binding_data(&root,&pin,&out)==PAPACC_RESULT_OK);
 C(papacc_configuration_q4_binding_current(&root,&pin,&out)==PAPACC_RESULT_OK);
 saved=out;
 for(i=0;i<sizeof(out);i++){((unsigned char*)&out)[i]^=1U;C(papacc_configuration_q4_binding_current(&root,&pin,&out)!=PAPACC_RESULT_OK);out=saved;}
 root.stopped=1;C(papacc_configuration_q4_binding_current(&root,&pin,&out)!=PAPACC_RESULT_OK);root.stopped=0;
 pin.ticket++;C(papacc_configuration_q4_binding_current(&root,&pin,&out)!=PAPACC_RESULT_OK);pin.ticket--;
 root.slots[0].view.revision++;C(papacc_configuration_q4_binding_current(&root,&pin,&out)!=PAPACC_RESULT_OK);root.slots[0].view.revision--;
 C(papacc_configuration_q4_binding_current(&root,&pin,(const PAPACC_Q4_ROOT_BINDING_DATA*)&root)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_configuration_q4_binding_current(&root,&pin,NULL)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(tc_tests()==0);reset();C(papacc_configuration_q4_st20_designation_data(&root,&pin,&designation)==PAPACC_RESULT_OK);C(designation.custodian_designated==1U&&designation.source_designated==1U&&designation.credential_access_denied==1U&&designation.source_type==13U);C(!strcmp(designation.custodian_sid,"S-1-5-21-785699550-1440458426-1682014700-1108"));root.initialized=0;DeleteCriticalSection(&root.lock);printf("PASS checks=%u binding_bytes=%zu root_bytes=%zu NO_STORE_NO_TPM_NO_AUTHORITY\n",checks,sizeof(out),sizeof(root));return 0;}
