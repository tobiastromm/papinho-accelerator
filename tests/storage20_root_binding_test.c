/* Root/pin inputs and native authorization are explicitly modeled; Windows
 * locking/threads and production root/binding/delivery code are real. No TPM. */
#include "storage20_binding.h"
#define main st20_prior_root_data_tests
#include "q4_root_binding_data_test.c"
#undef main
static TP_OWNER endpoint;
static ST20_DELIVERY delivery;
static ST20_ROOT_BINDING binding;
static unsigned source_calls,consumer_calls,deny_boundary,stop_during_boundary;
static TBS_RESULT WINAPI source_mock(TBS_HCONTEXT c,TBS_OWNERAUTH_TYPE kind,PBYTE out,PUINT32 n){source_calls++;if(c!=(void *)1||kind!=13||*n<4)return 99;memset(out,0x5a,4);*n=4;return 0;}
static TBS_RESULT WINAPI wrong_source(TBS_HCONTEXT c,TBS_OWNERAUTH_TYPE k,PBYTE p,PUINT32 n){(void)c;(void)k;(void)p;(void)n;source_calls+=1000U;return 0x80284001UL;}
static int native_boundary(TP_OWNER *o,const char *phase){(void)o;(void)phase;if(stop_during_boundary)root.stopped=1;return !deny_boundary;}
static int known_borrow(TP_OWNER *o,const unsigned char *p,unsigned n){(void)o;consumer_calls++;return n==4&&p[0]==0x5a;}
static int setup(unsigned char *scratch){reset();root.stopped=0;root.authority_lost=0;root.slots[0].retained=0;root.prepared=0;root.pending_commit.active=0;tp_init(&endpoint);endpoint.context=(void *)1;endpoint.boundary=native_boundary;memset(&binding,0,sizeof(binding));binding.root_owner=&root;binding.pin=&pin;
 if(papacc_configuration_q4_st20_designation_data(&root,&pin,&binding.expected)!=PAPACC_RESULT_OK)return 0;
 binding.designated_source=source_mock;memcpy(binding.current_sid,binding.expected.custodian_sid,80);memcpy(binding.current_ek,binding.expected.ek_spki,32);memcpy(binding.current_certificate,binding.expected.certificate,32);binding.request_digest[0]=1;binding.approval_digest[0]=2;
 memset(&delivery,0,sizeof(delivery));delivery.owner=&endpoint;delivery.original_context=(void *)1;delivery.buffer=scratch;delivery.capacity=64;delivery.guard_context=&binding;source_calls=consumer_calls=deny_boundary=stop_during_boundary=0;return 1;}
int main(void){ST20_API api;unsigned char *scratch;unsigned i,prior;PAPACC_Q4_ST20_DESIGNATION_DATA now,saved;
 C(st20_windows_source(NULL)==NULL);C(st20_windows_source(GetModuleHandleW(L"kernel32.dll"))==NULL);C(st20_prior_root_data_tests()==0);memset(&root,0,sizeof(root));InitializeCriticalSection(&root.lock);scratch=(unsigned char *)VirtualAlloc(NULL,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);C(scratch!=NULL);
 api.get=source_mock;api.lock=VirtualLock;api.unlock=VirtualUnlock;api.guard=st20_root_guard;api.use=known_borrow;api.clear=st20_secure_clear;
 C(setup(scratch));C(binding.expected.source_type==13&&binding.expected.credential_access_denied==1&&binding.expected.custodian_designated==1&&binding.expected.source_designated==1);
 C(st20_deliver(&delivery,&api));C(source_calls==1&&consumer_calls==1&&delivery.wiped&&!delivery.locked);C(!st20_deliver(&delivery,&api)&&source_calls==1);
 for(i=0;i<14;i++){
 C(setup(scratch));api.get=source_mock;
 switch(i){case 0:binding.current_sid[0]^=1;break;case 1:binding.current_ek[0]^=1;break;case 2:binding.current_certificate[0]^=1;break;case 3:binding.expected.source_decision[0]^=1;break;case 4:binding.expected.custodian_decision[0]^=1;break;case 5:binding.approval_digest[0]=0;break;case 6:binding.request_digest[0]=0;break;case 7:binding.expected.root.ticket++;break;case 8:root.stopped=1;break;case 9:deny_boundary=1;break;case 10:endpoint.boundary=NULL;break;case 11:api.get=wrong_source;break;case 12:stop_during_boundary=1;break;case 13:delivery.original_context=(void *)2;break;}
 if(st20_deliver(&delivery,&api)){printf("UNEXPECTED_ACCEPT model_case=%u\n",i);C(0);}
 C(source_calls==0&&consumer_calls==0);}
 C(setup(scratch));saved=binding.expected;memset(&now,0xa5,sizeof(now));prior=checks;root.stopped=1;C(papacc_configuration_q4_st20_designation_data(&root,&pin,&now)!=PAPACC_RESULT_OK);C(((unsigned char *)&now)[0]==0xa5);root.stopped=0;
 C(papacc_configuration_q4_st20_designation_data(&root,&pin,(PAPACC_Q4_ST20_DESIGNATION_DATA *)&root)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(setup(scratch));delivery.buffer=(unsigned char *)&root;C(!st20_deliver(&delivery,&api)&&source_calls==0);
 C(setup(scratch));delivery.buffer=(unsigned char *)&binding;C(!st20_deliver(&delivery,&api)&&source_calls==0);
 C(VirtualFree(scratch,0,MEM_RELEASE));DeleteCriticalSection(&root.lock);
 printf("PASS checks=%u prior_tail=%u designation_bytes=%zu binding_bytes=%zu delivery_bytes=%zu; ROOT_GUARDS_REAL_AUTHORITY_MODELED; OWNERAUTH_REAL_READS=0; NO_NV\n",checks,prior,sizeof(saved),sizeof(binding),sizeof(delivery));return 0;}
