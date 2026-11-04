#define _WIN32_WINNT 0x0600
#include "server_configuration_commit_win32.h"
#include "server_issuer_x509_win32.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static int eligible=1,target_ok=1,validate_ok=1;
#ifdef PAPACC_PERSONAL_CA_MODEL
static int personal_native_ceremony_ready(const PAPACC_CONFIG_PENDING *p){(void)p;return eligible;}
static PAPACC_RESULT personal_packet_current(PAPACC_CONFIG_PENDING *p){(void)p;return PAPACC_RESULT_OK;}
static PAPACC_RESULT personal_packet_source(PAPACC_CONFIG_PENDING *p){(void)p;return PAPACC_RESULT_OK;}
static BOOL WINAPI model_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f){(void)h;memset(f,0,sizeof(*f));f->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;f->dwVolumeSerialNumber=20;f->nFileIndexHigh=30;f->nFileIndexLow=target_ok?40:41;return TRUE;}
static SECURITY_STATUS model_validate(PAPACC_CA_KEY *k){(void)k;return validate_ok?ERROR_SUCCESS:NTE_BAD_KEY_STATE;}
#define papacc_configuration_personal_ca_match_data papacc_configuration_personal_ca_match_data_model
#define papacc_configuration_personal_ca_bind papacc_configuration_personal_ca_bind_model
#define papacc_configuration_personal_ca_current papacc_configuration_personal_ca_current_model
#define papacc_configuration_personal_ca_release papacc_configuration_personal_ca_release_model
#define papacc_configuration_personal_ca_handoff papacc_configuration_personal_ca_handoff_model
#define GetFileInformationByHandle model_file
#define papacc_ca_key_validate model_validate
#include "../apps/server/server_configuration_personal_ca_win32.inc"
#undef papacc_ca_key_validate
#undef GetFileInformationByHandle
#endif
static PAPACC_CONFIGURATION_COMMIT o;
static PAPACC_CA_KEY ca;
static unsigned char id[32]={1},authority[16]={1},generation[16]={2},op[4]={1,2,3,4},svc[4]={4,3,2,1};
#define CHECK(x) do{if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
static int setup(void)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC v;
 unsigned char parent[12]={10,0,0,0,11,0,0,0,12,0,0,0};size_t bytes,i,z;char reply[128];static const char hex[]="0123456789abcdef",prefix[]="OWNER_NEW_INSTALL_CA_INITIALIZE ";
 static const unsigned char config[]="format_version 1\nclient_admission DISABLED\n";
 memset(&o,0,sizeof(o));memset(&ca,0,sizeof(ca));p=&o.pending_commit;n=&p->personal_native;o.writer=GetCurrentThreadId();o.initialized=1;
 n->phase=1;n->handoff_state=PAPACC_PERSONAL_HANDOFF_VERIFIED;memcpy(n->handoff_attempt,id,32);memcpy(n->plan_leaf,"child",6);memcpy(p->previous,"child",6);memcpy(p->next,config,sizeof(config)-1);
 n->parent.dwVolumeSerialNumber=10;n->parent.nFileIndexHigh=11;n->parent.nFileIndexLow=12;memcpy(n->sid,op,4);n->sid_bytes=4;
 p->bootstrap_created=1;p->bootstrap_parent=(HANDLE)(ULONG_PTR)1;p->bootstrap_container=(HANDLE)(ULONG_PTR)2;p->bootstrap_volume=20;p->bootstrap_high=30;p->bootstrap_low=40;p->result=PAPACC_CAS_SUCCESS;
 ca.thread=o.writer;ca.state=PAPACC_CA_KEY_READY;ca.key=(NCRYPT_KEY_HANDLE)3;ca.provider=(NCRYPT_PROV_HANDLE)4;ca.created=1;ca.diagnostic_finalization=2;ca.operator_size=4;ca.service_size=4;memcpy(ca.operator_sid,op,4);memcpy(ca.service_sid,svc,4);
 wcscpy_s(ca.name,96,L"PapinhoAccelerator.CA.01000000000000000000000000000000.02000000000000000000000000000000");
 memset(&v,0,sizeof(v));v.operator_id=op;v.operator_bytes=4;v.parent_id=parent;v.leaf=(const unsigned char *)"child";v.leaf_bytes=5;v.authority=authority;v.generation=generation;v.service_id=svc;v.service_bytes=4;v.configuration=config;v.configuration_bytes=sizeof(config)-1;
 bytes=papacc_personal_effect_build(&v,p->previous+1024,PAPACC_PERSONAL_PLAN_MAX);if(!bytes)return 0;
 if(!papacc_personal_data_capture(&p->personal_data,id,config,sizeof(config)-1)||!papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024,bytes)||!papacc_personal_data_present(&p->personal_data))return 0;
 z=sizeof(prefix)-1;memcpy(reply,prefix,z);for(i=0;i<32;i++){reply[z+i*2]=hex[id[i]>>4];reply[z+i*2+1]=hex[id[i]&15];}
 return papacc_personal_effect_confirm(&p->personal_data,id,p->previous+1024,bytes,reply,z+64)&&papacc_personal_effect_consume(&p->personal_data,id,p->previous+1024,bytes);
}
int main(void)
{
 PAPACC_CONFIG_PENDING *p=&o.pending_commit;unsigned char other[32]={2};PAPACC_RESULT r=PAPACC_RESULT_OK;
 CHECK(setup());CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_match_data(&o,NULL,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,other,authority,generation)!=PAPACC_RESULT_OK);
 ca.operator_sid[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.operator_sid[0]^=1;
 p->personal_native.parent.nFileIndexLow^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);p->personal_native.parent.nFileIndexLow^=1;
 p->next[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);p->next[0]^=1;
 ca.name[22]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.name[22]^=1;
 ca.service_sid[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.service_sid[0]^=1;
 ca.provider=0;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.provider=4;
 wcscpy_s(ca.name,96,L"PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0");CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)==PAPACC_RESULT_NOT_SUPPORTED);
 wcscpy_s(ca.name,96,L"PapinhoAccelerator.CA.01000000000000000000000000000000.02000000000000000000000000000000");
 o.stopped=1;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);o.stopped=0;
#ifdef PAPACC_PERSONAL_CA_MODEL
 target_ok=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);target_ok=1;
 validate_ok=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);validate_ok=1;
 eligible=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);eligible=1;
 CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK&&p->personal_ca==&ca&&ca.borrower==p);
 CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_ca_key_close(&ca)==NTE_BAD_KEY_STATE&&ca.key&&ca.provider); /* real borrow guard, no native FreeObject */
 CHECK(papacc_configuration_personal_ca_current(&o,id,authority,generation)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_current(&o,other,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_native_close(&o)!=PAPACC_RESULT_OK);CHECK(papacc_configuration_personal_data_retire(&o)!=PAPACC_RESULT_OK);
 p->active=1;CHECK(papacc_configuration_personal_ca_release(&o)!=PAPACC_RESULT_OK&&p->personal_ca==&ca);p->active=0;
 o.stopped=1;p->stop=1;CHECK(papacc_configuration_personal_ca_current(&o,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_release(&o)==PAPACC_RESULT_OK&&!p->personal_ca&&!ca.borrower);
 CHECK(papacc_configuration_personal_ca_release(&o)!=PAPACC_RESULT_OK);
 o.stopped=0;p->stop=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(setup());CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 {PAPACC_ISSUER_CNG_BACKEND backend;PAPACC_ISSUER_CORE core;memset(&backend,0,sizeof(backend));memset(&core,0,sizeof(core));
  backend.initialized=1;backend.thread=o.writer;backend.key=&ca;backend.owner=&core;backend.ports.ctx=&backend;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,other,authority,generation)!=0&&p->personal_ca==&ca&&ca.borrower==p&&!backend.custody);
  backend.key=NULL;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);backend.key=&ca;
  validate_ok=0;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p&&!backend.custody);validate_ok=1;
  target_ok=0;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);target_ok=1;
  eligible=0;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);eligible=1;
  backend.active=1;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);backend.active=0;
  core.initialized=1;core.continuity_profile=PAPACC_ISSUER_CONTINUITY_RECOVERY;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);core.initialized=0;
  core.initialized=1;core.continuity_profile=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);core.initialized=0;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)==0&&ca.borrower==&backend&&!p->personal_ca&&backend.custody);
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==&backend);
  CHECK(papacc_configuration_personal_ca_release(&o)!=0&&ca.borrower==&backend); /* source cannot release recipient */
  CHECK(papacc_ca_key_return(&ca,&backend)==0); /* model recipient returns lifetime reference, no native key close */
 }
 CHECK(setup());CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 ca.state=PAPACC_CA_KEY_RETAINED;CHECK(papacc_configuration_personal_ca_release(&o)!=PAPACC_RESULT_OK&&o.unresolved&&p->personal_ca==&ca&&ca.borrower==p);
#else
 (void)eligible;(void)target_ok;(void)validate_ok;
 r=papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation);CHECK(r==PAPACC_RESULT_NOT_SUPPORTED&&!p->personal_ca&&!ca.borrower);
 CHECK(papacc_configuration_personal_ca_bind(&o,NULL,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_current(&o,id,authority,generation)!=PAPACC_RESULT_OK);
#endif
 (void)r;CHECK(!o.published&&!o.committed_bytes&&!o.committed_pending);
 printf("PASS CA_BINDING_%s NO_KEYS_NO_AUTHORITY root=%zu pending=%zu ca=%zu\n",
#ifdef PAPACC_PERSONAL_CA_MODEL
 "MODEL",
#else
 "NATIVE_NEGATIVE",
#endif
 sizeof(o),sizeof(*p),sizeof(ca));return 0;
}
